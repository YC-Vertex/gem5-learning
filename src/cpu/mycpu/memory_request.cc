#include "cpu/mycpu/memory_request.hh"

#include "arch/locked_mem.hh"
#include "arch/mmapped_ipr.hh"
#include "arch/utility.hh"
#include "debug/MyCPU.hh"

MemoryRequest::MemoryRequest(MyCPU &cpu, SimpleThread &thread, StaticInstPtr inst,
    Addr addr, unsigned int size, Request::Flags flags,
    uint8_t *_data, uint64_t *res) :
    cpu(cpu), thread(thread), inst(inst), data(nullptr), res(res),
    isFetch(false), isRead(_data ? false : true), isSplit(false),
    req(nullptr), pkt(nullptr), fault(NoFault),
    sreqLow(nullptr), sreqHigh(nullptr), spktLow(nullptr), spktHigh(nullptr),
    waitingFor(0)
{
    req = new Request(0, addr, size, flags, cpu.dataMasterId(),
        thread.instAddr(), thread.contextId());
    
    Addr split_addr = roundDown(addr + size - 1, cpu.cacheLineSize());
    assert(split_addr <= addr || split_addr - addr < cpu.cacheLineSize());

    if (split_addr > addr) {
        isSplit = true;
        assert(!req->isLLSC() && !req->isSwap());
        req->splitOnVaddr(split_addr, sreqLow, sreqHigh);
    }

    if (!isRead) {
        data = new uint8_t[size];
        memcpy(data, _data, size);
    }
}

MemoryRequest::MemoryRequest(MyCPU &cpu, SimpleThread &thread, Addr addr) :
    cpu(cpu), thread(thread), addr(addr)
{
    req = new Request(0, inst_addr, sizeof(TheISA::MachInst),
        Request::INST_FETCH, cpu.instMasterId(),
        thread.instAddr(), thread.contextId());

    if (((inst_addr + sizeof(TheISA::MachInst) - 1) / cpu.cacheLineSize()) >
        (inst_addr / cpu.cacheLineSize())) {
        panic("MyCPU can not deal with fetches across a cache line boundary\n");
    }
}

MemoryRequest::~MemoryRequest()
{
    delete req;
    if (pkt) delete pkt;
}

void
MemoryRequest::translate()
{
    if (isFetch) {
        DPRINTFS(MyCPU, (&cpu),
            "Fetching addr %#x (+%#x)\n",
            thread.instAddr(),
            req->getVaddr() - thread.instAddr());
        thread.itb->translateTiming(req, thread.getTC(), this,
            BaseTLB::Execute);
    } else {
        DPRINTFS(MyCPU, (&cpu),
            "%s addr %#x (size: %d)\n",
            isRead ? "Read" : "Write",
            req->getVaddr(), req->getSize());
        if (isSplit) {
            waitingFor = 2;
            thread.dtb->translateTiming(sreqLow, thread.getTC(), this,
                isRead ? BaseTLB::Read : BaseTLB::Write);
            thread.dtb->translateTiming(sreqHigh, thread.getTC(), this,
                isRead ? BaseTLB::Read : BaseTLB::Write);
        } else {
            thread.dtb->translateTiming(req, thread.getTC(), this,
                isRead ? BaseTLB::Read : BaseTLB::Write);
        }
    }
}

void
MemoryRequest::finish(const Fault &_fault, RequestPtr req,
    ThreadContext *tc, BaseTLB::Mode mode)
{
    if (fault == NoFault && _fault != NoFault) {
        fault = _fault;
    }

    if (isSplit) {
        waitingFor--;
        this->req->setFlags(req->getFlags());
        if (waitingFor != 0) {
            DPRINTFS(MyCPU, (&cpu), "Waiting for another request\n");
            return;
        }
        this->req->setPaddr(sreqLow->getPaddr());
    }

    if (isFetch)
        cpu.finishFetchTranslate(this);
    else
        cpu.finishDataTranslate(this);
}

void
MemoryRequest::send()
{
    assert(req && pkt == nullptr);

    if (isFetch) sendFetch();
    else if (isSplit) sendDataSplit();
    else sendData();
}

void
MemoryRequest::sendFetch()
{
    assert(isFetch);
    DPRINTFS(MyCPU, (&cpu), "Sending fetch for %#x (phys addr: %#x)\n)",
        req->getVaddr(), req->getPaddr());
    
    pkt = new Packet(req, MemCmd::ReadReq);
    pkt->allocate();

    auto *port = dynamic_cast<MyCPU::IcachePort>(&cpu.getInstPort());
    assert(port);
    port->sendPacket(this, pkt);
}

void
MemoryRequest::sendData()
{
    bool do_access = true;
    if (isRead) {
        DPRINTFS(MyCPU, (&cpu), "Sending read for %#x (phys addr: %#x)\n",
            req->getVaddr(), req->getPaddr());

        if (req->isLLSC()) {
            TheISA::handleLockedRead(&thread, req);
        }

        pkt = Packet::createRead(req);
        pkt->allocate();
    } else {
        DPRINTFS(MyCPU, (&cpu), "Sending write for %#x (phys addr: %#x)\n"
            req->getVaddr(), req->getPaddr());

        if (req->isLLSC()) {
            do_access = TheISA::handleLockedWrite(&thread, req,
                ~((Addr)cpu.cacheLineSize() - 1));
            DPRINTFS(MyCPU, (&cpu), "Doing LLSC. %s access\n",
                do_access ? "Should" : "Should NOT");
        } else if (req->isCondSwap()) {
            assert(res);
            req->setExtraData(*res);
        }

        pkt = Packet::createWrite(req);
        assert(data);
        pkt->dataDynamic<uint8_t>(data);
    }

    if (do_access) {
        auto *port = dynamic_cast<MyCPU::DcachePort>(&cpu.getDataPort());
        assert(port);
        port->sendPacket(this, pkt);
    } else {
        cpu.dataResponse(pkt, inst);
    }
}

void
MemoryRequest::sendDataSplit()
{
    assert(!req->isLLSC() && !sreqLow->isLLSC() && !sreqHigh->isLLSC());
    assert(!req->isCondSwap && !sreqLow->isCondSwap() && !sreqHigh->isCondSwap());

    if (isRead) {
        spktLow = Packet::createRead(sreqLow);
        spktHigh = Packet::createRead(sreqHigh);
    } else {
        spktLow = Packet::createWrite(sreqLow);
        spktHigh = Packet::createWrite(sreqHigh);
    }

    pkt = new Pakcet(req, spktLow->cmd.responseCommand());

    if (isRead) {
        pkt->allocate();
        data = pkt->getPtr<uint8_t>(data);
    } else {
        pkt->dataDynamic<uint8_t>(data);
    }
    spktLow->dataStatic<uint8_t>(data);
    spktHigh->dataStatic<uint8_t>(data + sreqLow->getSize());

    waitingFor = 2;

    auto *port = dynamic_cast<MyCPU::DcachePort*>(&cpu.getDataport());
    assert(port);
    port->sendPacket(this, spktLow);
}

void
MemoryRequest::recvResponse(PacketPtr _pkt)
{
    if (isSplit)  {
        DPRINTFS(MyCPU, (&cpu), "Got split response %s\n",
            _pkt->print());
        waitingFor--;
        if (waitingFor > 0) {
            DPRINTFS(MyCPU, (&cpu), "Waiting for another request\n");
            assert(waitingFor == 1);
            auto *port = dynamic_cast<MyCPU::DcachePort>(&cpu.getDataPort());
            assert(port);
            port->sendPacket(this, spktHigh(;
            return true;
        }
        delete spktLow;
        delete spktHigh;
        delete sreqLow;
        delete sreqHigh;
    } else {
        assert(_pkt == pkt);
    }

    if (isFetch)
        cpu.decodeInstruction(pkt);
    else
        cpu.dataResponse(pkt, inst);

    delete this;
    return true;
}

void
MemoryRequest::trace(Trace::InstRecord *trace)
{
    trace->setMem(req->getVaddr(), req->getSize(), req->getFlags());
}


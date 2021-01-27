#include "cpu.hh"

#include "arch/locked_mem.hh"
#include "arch/mmapped_ipr.hh"
#include "arch/utility.hh"
#include "debug/MyCPU.hh"

MyCPU::MyCPU(MyCPUParams *params) :
    BaseCPU(params),
    dport(this), iport(this),
    thread(this, 0, params->system, params->workload[0],
        params->itb, params->dtb, params->isa[0]),
    currentMacroOp(nullptr),
    traceData(nullptr)
{
    fatal_if(FullSystem, "MyCPU does not support full system.");
    threadContexts.push_back(thread.getTC());
}

void
MyCPU::init()
{
    DPRINTF(MyCPU, "MyCPU init\n");

    BaseCPU::init();
    thread.getTC()->initMemProxies(thread.getTC());
}

void
MyCPU::startup()
{
    DPRINTF(MyCPU, "MyCPU startup\n");

    BaseCPU::startup();
    thread.startup();
}

void
MyCPU::wakeup(ThreadID tid)
{
    assert(tid == 0);

    DPRINTF(MyCPU, "MyCPU wakeup\n");

    if (thread.status() == ThreadContext::Suspended) {
        DPRINTF(MyCPU, "[tid:%d] Suspended Processor awoke\n", tid);
        thread.activate();
    }
}

void
MyCPU::activateContext(ThreadID tid)
{
    DPRINTF(MyCPU, "ActivateContext thread: %d\n", tid);

    BaseCPU::activateContext(tid);
    thread.activate();

    auto e = new EventFunctionWrapper(
        [this] { fetch(); },
        name + "initial_fetch",
        true);
    schedule(e, true);
}

void
MyCPU::CPUPort::sendPacket(MemoryRequest *request, PacketPtr pkt)
{
    panic_if(blockedPacket != nullptr, "Should never try to send if blocked!");

    assert(outstandingRequest == nullptr);
    outstandingRequest = request;

    DPRINTF(MyCPU, "Sending packet %s\n", pkt->print());

    if (!sendTimingReq(pkt)) {
        blockedPacket = pkt;
    }
}

bool
MyCPU::CPUPort::recvTimingResp(PacketPtr pkt)
{
    DPRINTF(MyCPU, "Got response for addr %#x\n", pkt->getAddr());

    assert(outstandingRequest);
    MemoryRequest *request = outstandingRequest;
    outstandingRequest = nullptr;
    return request->recvResponse(pkt);
}

void
MyCPU::CPUPort::recvReqRetry()
{
    assert(blockedPacket != nullptr);
    assert(outstandingRequest);

    DPRINTF(MyCPU, "Got retry request\n");

    PacketPtr pkt = blockedPacket;
    blockedPacket = nullptr;

    MemoryRequest *request = outstandingRequest;
    outstandingRequest = nullptr;

    sendPacket(request, pkt);
}

void
MyCPU::fetch(Addr offset)
{
    Addr inst_addr = thread.instAddr() & BaseCPU::PCMask;

    MemoryRequest *request
        = new MemoryRequest(*this, thread, inst_addr + offset);
    
    request->translate();
}

void
MyCPU::finishFetchTranslate(MemoryRequest *request)
{
    if (request->getFault() != NoFault) {
        DPRINTF(MyCPU, "Translation of addr %#x faulted\n)",
            request->getReq()->getVaddr());
        delete request;
        panic("Currently MyCPU does not support fetch faults\n");
    } else {
        panic_if(port.isBlocked(), "Do not know what to do if the port is blocked");
        request->send();
    }
}

void
MyCPU::decodeInstruction(PacketPtr pkt)
{
    DPRINTF(MyCPU, "Decoding the instruction\n");

    thread.decoder.moreBytes(thread.pcState(), pkt->req->getVaddr(),
        *pkt->getConstPtr<TheISA::MachInst>());
    TheISA::PCState next_pc = thread.pcState();

    StaticInstPtr inst = thread.decoder.decode(next_pc);

    if (!inst) {
        DPRINTF(MyCPU, "Need to fetch more data\n");
        schedule(new EventFunctionWrapper(
                [this, pkt] {
                    Addr base = (thread.instAddr() & BaseCPU::PCMask);
                    Addr cur_offset = pkt->req->getVaddr() - base;
                    fetch(cur_offset + sizeof(TheISA::MachInst);
                },
                name() + ".extra_fetch",
                true),
            curTick());

        return;
    }

    thread.pcState(next_pc);

    executeInstruction(inst);
}

void
MyCPU::executeInstruction(StaticInstPtr inst)
{
    DPRINTF(MyCPU, "Executing the instruction\n");

    StaticInstPtr macroop = NULL;
    if (inst->isMacroop()) {
        assert(!currentMacroOp);
        macroop = inst;
        DPRINTF(MyCPU, "Decomposing a macro-op\n");
        inst = inst->fetchMicroop(thread.pcState().microPC());
        if (!inst->isLastMicroop()) {
            currentMacroOp = macroop;
        }
    }

# if TRACING_ON
    traceData = getTracer()->getInstRecord(curTick(),
        thread.getTC(), inst, thread.pcState(), macroop);
# endif

    thread.setIntReg(TheISA::ZeroReg, 0);

    MyCPUContext exec_context(*this, thread, inst);

    if (inst->isMemRef()) {
        DPRINTF(MyCPU, "Found a memory instruction\n");
        Fault fault = inst->initiateAcc(&exec_context, traceData);
        panic_if(fault != NoFault, "Do not know how to deal with mem faults!\n");
    } else {
        DPRINTF(MyCPU, "Found a non-memory instruction\n");
        Fault fault = inst->execute(&exec_context, traceData);
        finishExecute(inst, fault);
    }
}

void 
MyCPU::finishDataTranslate(MemoryRequest *request)
{
    if (traceData) {
        request->trac(traceData);
    }

    if (requset->getFault() != NoFault) {
        DPRINTF(MyCPU, "Translation of addr %#x faulted\n",
            request->getReq()->getVaddr());
        delete request;
        panic("Currently MyCPU does not support memory faults\n");
        return;
    } else if (request->getReq()->getFlags().isSet(Request::NO_ACCESS)) {
        panic("Do not know how to deal with Request::NO_ACCESS\n");
    } else if (request->getReq()->isMmappedIpt()) {
        Cycles delay;
        PacketPtr pkt = request->getPkt();
        if (request->getIsRead()) {
            delay = TheISA::handleIprRead(thread.getTC(), pkt);
        } else {
            delay = TheISA::handleIprWrite(thread.getTC(), pkt);
        }
        
        schedule(new EventFunctionWrapper(
                [request, pkt] { request->recvResponse(pkt); },
                name() + ".ipr_delay",
                true),
            clockEdge(delay));
    } else {
        panic_if(port.isBlocked(), "Do not know what to do if the port is blocked\n");
        request->send();
    }
}

void
MyCPU::dataResponse(PacketPtr pkt, StaticInstPtr inst)
{
    assert(!pkt->isError());

    MyCPUContext exec_context(*this, thread, inst);

    Fault fault = inst->completeAcc(pkt, &exec_context, traceData);

    pnic_if(fault != NoFault, "Do not know how to handle this fault!\n");

    finishExecute(inst, fault);
}

void
MyCPU::finishExecute(StaticInstPtr inst, const Fault *fault)
{
    DPRINTF(MyCPU, "Finishing execute and moving to next inst\n");

    if (traceData) {
        traceData->dump();
        delete traceData;
        traceData = nullptr;
    }

    if (fault != NoFault) {
        DPRINTF(MyCPU, "Instruction faulted! Invoking ...\n");
        fault->invoke(thread.getTC(), inst);
        thread.decoder.reset();
    } else {
        TheISA::PCState pc_state = thread.pcState();
        TheISA::advancePC(pc_state, inst);
        thread.pcState(pc_state);
    }

    if (inst->isMicroop() && currentMacroOp) {
        DPRINTF(MyCPU, "Still more of the macro op to execute\n");
        inst = currentMacroOp;
        currentMacroOp = nullptr;
        schedule(new EventFunctionWrapper(
                [this, inst] { executeInstruction(inst); },
                name() + ".next_uop",
                true),
            nextCycle());
    } else {
        DPRINTF(MyCPU, "Fetching a new instruction\n");
        schedule(new EventFunctionWrapper(
                [this] { fetch(); },
                name() + ".initial_fetch",
                true),
            nextCycle());
    }
}

MyCPU*
MyCPUParams::create()
{
    return new MyCPU(this);
}


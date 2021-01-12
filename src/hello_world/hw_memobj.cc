#include "hello_world/hw_memobj.hh"
#include "debug/HwMemobj.hh"

#include "base/trace.hh"

HwMemobj::HwMemobj(HwMemobjParams *params) :
    SimObject(params),
    instPort(params->name + ".inst_port", this),
    dataPort(params->name + ".data_port", this),
    memPort(params->name + ".mem_side", this),
    blocked(false)
{
}

Port&
HwMemobj::getPort(const std::string &if_name, PortID idx)
{
    panic_if(idx != InvalidPortID, "This object doesn't support vector ports");

    if (if_name == "mem_side") {
        return memPort;
    } else if (if_name == "inst_port") {
        return instPort;
    } else if (if_name == "data_port") {
        return dataPort;
    } else {
        return SimObject::getPort(if_name, idx);
    }
}

// implement cpu-side port functions

AddrRangeList
HwMemobj::CPUSidePort::getAddrRanges() const
{
    return owner->getAddrRanges();
}

void
HwMemobj::CPUSidePort::recvFunctional(PacketPtr pkt)
{
    owner->handleFunctional(pkt);
}

AddrRangeList
HwMemobj::getAddrRanges() const
{
    DPRINTF(HwMemobj, "Sending new ranges\n");
    return memPort.getAddrRanges();
}

void
HwMemobj::handleFunctional(PacketPtr pkt)
{
    memPort.sendFunctional(pkt);
}

// implement mem-side port functions

void
HwMemobj::MemSidePort::recvRangeChange()
{
    owner->sendRangeChange();
}

void
HwMemobj::sendRangeChange()
{
    instPort.sendRangeChange();
    dataPort.sendRangeChange();
}

// implement receiving requests

bool
HwMemobj::CPUSidePort::recvTimingReq(PacketPtr pkt)
{
    if (!owner->handleRequest(pkt)) {
        needRetry = true;
        return false;
    } else {
        return true;
    }
}

bool
HwMemobj::handleRequest(PacketPtr pkt)
{
    if (blocked) {
        return false;
    }
    DPRINTF(HwMemobj, "Got request for add $#x\n", pkt->getAddr());
    blocked = true;
    memPort.sendPacket(pkt);
    return true;
}

void
HwMemobj::MemSidePort::sendPacket(PacketPtr pkt)
{
    panic_if(blockedPacket != nullptr, "Should never try to send if blocked");
    if (!sendTimingReq(pkt)) {
        blockedPacket = pkt;
    }
}

void
HwMemobj::MemSidePort::recvReqRetry()
{
    assert(blockedPacket != nullptr);
    
    PacketPtr pkt = blockedPacket;
    blockedPacket = nullptr;

    sendPacket(pkt);
}

// implement receiving responses

bool
HwMemobj::MemSidePort::recvTimingResp(PacketPtr pkt)
{
    return owner->handleResponse(pkt);
}

bool
HwMemobj::handleResponse(PacketPtr pkt)
{
    assert(blocked);
    DPRINTF(HwMemobj, "Got response for addr %#x\n", pkt->getAddr());

    blocked = false;

    if (pkt->req->isInstFetch()) {
        instPort.sendPacket(pkt);
    } else {
        dataPort.sendPacket(pkt);
    }

    instPort.trySendRetry();
    dataPort.trySendRetry();

    return true;
}

void
HwMemobj::CPUSidePort::sendPacket(PacketPtr pkt)
{
    panic_if(blockedPacket != nullptr, "Showld never try to send if blocked!");

    if (!sendTimingResp(pkt)) {
        blockedPacket = pkt;
    }
}

void
HwMemobj::CPUSidePort::recvRespRetry()
{
    assert(blockedPacket != nullptr);

    PacketPtr pkt = blockedPacket;
    blockedPacket = nullptr;

    sendPacket(pkt);
}

void
HwMemobj::CPUSidePort::trySendRetry()
{
    if (needRetry && blockedPacket == nullptr) {
        needRetry = false;
        DPRINTF(HwMemobj, "Sending retry req for %d\n", id);
        sendRetryReq();
    }
}

// constructor
HwMemobj*
HwMemobjParams::create()
{
    return new HwMemobj(this);
}

#include "cpu/mycpu/exec_context.hh"

IntReg
MyCPUContext::readIntRegOperand(const StaticInst *si, int idx)
{
    const RegId &reg = si->srcRegIdx(idx);
    assert(reg.isIntReg());
    return thread.readIntReg(reg.index());
}

void
MyCPUContext::setIntRegOperand(const StaticInst *si, int idx, IntReg val)
{
    const RegId &reg = si->destRegIdx(idx);
    assert(reg.isIntReg());
    thread.setIntReg(reg.index(), val);
}

FloatReg
MyCPUContext::readFloatRegOperand(const StaticInst *si, int idx)
{
    const RegId &reg = si->srcRegIdx(idx);
    assert(reg.isFloatReg());
    return thread.readFloatReg(reg.index());
}

void
MyCPUContext::setFloatRegOperand(const StaticInst *si, int idx, FloatReg val)
{
    const RegId &reg = si->destRegIdx(idx);
    assert(reg.isFloatReg());
    thread.setFloatReg(reg.index(), val);
}

FloatRegBits
MyCPUContext::readFloatRegOperandBits(const StaticInst *si, int idx)
{
    const RegId &reg = si->srcRegIdx(idx);
    assert(reg.isFloatReg());
    return thread.readFloatRegBits(reg.index());
}

void
MyCPUContext::setFloatRegOperandBits(const StaticInst *si, int idx, IntRegBits val)
{
    const RegId &reg = si->destRegIdx(idx);
    assert(reg.isFloatReg());
    thread.setFloatRegBits(reg.index(), val);
}

const VecRegContainer&
MyCPUContext::readVecRegOperand(const StaticInst *si, int idx) const
{
    const RegId &reg = si->srcRegIdx(idx);
    assert(reg.isVecReg());
    return thread.readVecReg(reg.index());
}

VecRegContainer&
MyCPUContext::getWritableVecRegOperand(const StaticInst *si, int idx)
{
    const RegId &reg = si->destRegIdx(idx);
    assert(reg.isVecReg());
    thread.setVecReg(reg, val);
}

void
MyCPUContext::setVecRegOperand(const StaticInst *si, int idx, const VecRegContainer &val)
{
    const RegId &reg = si->destRegIdx(idx);
    assert(reg.isVecReg());
    thread.setVecReg(reg, val);
}

template <typename VecElem>
VecLaneT<VecElem, true>
MyCPUContext::readVecaLaneOperand(const StaticInst *si, int idx) const
{
    const RegId &reg = si->srcRegIdx(idx);
    assert(reg.isVecReg());
    return thread.readVecLane<VecElem>(reg);
}

template <typename LD>
void
MyCPUContext::setVecLaneOperandT(const StaticInst *si, int idx, const LD &val)
{
    const RegId &reg = si->destRegIdx(idx);
    assert(reg.isVecReg());
    thread.setVecLane(reg, val);
}

VecElem
MyCPUContext::readVecElemOperand(const StaticInst *si, int idx) const
{
    const RegId &reg = si->destRegIdx(idx);
    assert(reg.isVecElem())
    return thread.readVecElem(reg);
}

void
MyCPUContext::setVecElemOperand(const StaticInst *si, int idx, const VecElem val)
{
    const RegId &reg = si->destRegIdx(idx);
    assert(reg.isVecElem());
    thread.setVecElem(reg, val);
}

CCReg
MyCPUContext::readCCRegOperand(const StatisInst *si, int idx)
{
    const RegId &reg = si->srcRegIdx(idx);
    assert(reg.isCCReg());
    return thread.readCCReg(reg.index());
}

void
MyCPUContext::setCCRegOperand(const StaticInst *si, int idx, const CCReg val)
{
    const RegId &reg = si->destRegIdx(idx);
    assert(reg.isCCReg());
    thread.setCCReg(reg.index(), val);
}

MiscReg
MyCPUContext::readMiscRegOperand(const StatisInst *si, int idx)
{
    const RegId &reg = si->srcRegIdx(idx);
    assert(reg.isMiscReg());
    return thread.readMiscReg(reg.index());
}

void
MyCPUContext::setMiscRegOperand(const StaticInst *si, int idx, const MiscReg val)
{
    const RegId &reg = si->destRegIdx(idx);
    assert(reg.isMiscReg());
    thread.setMiscReg(reg.index(), val);
}

Fault initiateMemRead(Addr addr, unsigned int size, Request::Flags flags)
{
    MemoryRequest *request
        = new MemoryRequest(cpu, thread, inst, addr, size, flags);
    request->translate();
    return NoFault;
}

Fault writeMem(uint8_t *data, unsigned int size, Addr addr,
    Request::Flags flags, uint64_t *res)
{
    MemoryRequest *request
        = new MemoryRequest(cpu, thread, inst, addr, size, flags, data, res);
    request->translate();
    return NoFault;
}


#ifndef __MYCPU_EXEC_CONTEXT_HH__
#define __MYCPU_EXEC_CONTEXT_HH__

#include "cpu/base.hh"
#include "cpu/exec_context.hh"
#include "cpu/mycpu/cpu.hh"
#include "cpu/simple_thread.hh"
#include "mem/request.hh"

class MyCPUContext : public ExecContext
{
  private:
    MyCPU &cpu;
    SimpleThread &thread;
    StaticInstPtr inst;

  public:
    MyCPUContext(MyCPU &cpu, SimpleThread &thread, StaticInstPtr inst) :
        cpu(cpu), thread(thread), inst(inst)
    { }

    // integer register interfaces
    IntReg readIntRegOperand(const StaticInst *si, int idx) override;
    void setIntRegOperand(const StaticInst *si, int idx, IntReg val) override;

    // floating point register interfaces
    FloatReg readFloatRegOperand(const StaticInst *si, int idx) override;
    void setFloatRegOperand(const StaticInst *si, int idx, IntReg val) override;
    FloatRegBits readFloatRegOperandBits(const StaticInst *si, int idx) override;
    void setFloatRegOperandBits(const StaticInst *si, int idx, FloatRegBits val) override;

    // vector register interfaces
    const VecRegContainer &readVecRegOperand(const StaticInst *si, int idx) override;
    VecRegContrainer &getWritableVecRegOperand(const StaticInst *si, int idx) override;
    void setVecRegOperand(cosnt StaticInst *si, int idx, const VecRegContainer &val) override;

    // vector register lane interfaces
    template <typename VecElem>
    VecLaneT<VecElem, true> readVecLaneOperand(const StaticInst *si, int idx) const;

    ConstVecLane8
    readVec8BitLaneOperand(const StaticInst *si, int idx) const override
    { return readVecLaneOperand<uint8_t>(si, idx); }

    ConstVecLane16
    readVec16BitLaneOperand(const StaticInst *si, int idx) const override
    { return readVecLaneOperand<uint16_t>(si, idx); }

    ConstVecLane32
    readVec32BitLaneOperand(const StaticInst *si, int idx) const override
    { return readVecLaneOperand<uint32_t>(si, idx); }

    ConstVecLane64
    readVec64BitLaneOperand(const StaticInst *si, int idx) const override
    { return readVecLaneOperand<uint64_t>(si, idx); }

    template <typename LD>
    void setVecLaneOperandT(const StaticInst *si, int idx, const LD &val);

    void setVecLaneOperand(const  StaticInst *si, int idx,
        const LaneData<LaneSize:Byte> &val) override
    { setVecLaneOperandT(si, idx, val); }

    void setVecLaneOperand(const  StaticInst *si, int idx,
        const LaneData<LaneSize:TwoByte> &val) override
    { setVecLaneOperandT(si, idx, val); }

    void setVecLaneOperand(const  StaticInst *si, int idx,
        const LaneData<LaneSize:FourByte> &val) override
    { setVecLaneOperandT(si, idx, val); }

    void setVecLaneOperand(const  StaticInst *si, int idx,
        const LaneData<LaneSize:EightByte> &val) override
    { setVecLaneOperandT(si, idx, val); }

    // vector elem interfaces
    virtual VecElem raeadVecElemOperand(const StaticInst *si, int idx) const override;
    virtual void setVecElemOperand(const StaticInst *si, int idx, const VecElem val) override;

    // condition code register interfaces
    CCReg readCCRegOperand(const StaticInst *si, int idx) override;
    void setCCRegOperand(const StaticInst *si, int idx, CCReg val) override;

    // miscellaneous register interfaces
    MiscReg readMiscRegOperand(const StaticInst *si, int idx) override;
    void setMiscRegOperand(const StaticInst *si, int idx, const MiscReg &val) override;

    MiscReg readMiscReg(int misc_reg) override
    { return thread.readMiscReg(misc_reg); }

    void setMiscReg(int misc_reg, const MiscReg &val) override
    { return thread.setMiscReg(misc_reg, val); }

    // pc control
    PCState pcState() const override
    { return thread.pcState(); }

    void pcState(cosnt PCState &val) override
    { thread.pcState(val); }

    // memory interfaces
    Fault readMem(Addr addr, uint8_t *data, unsigned int size,
        Request::Flags flags) override
    { panic("MyCPU does not support atomic accesses."); }

    Fault initiateMemRead(Addr addr, unsigned int size,
        Request::Flags flags) override;
    Fault writeMem(uint8_t *data, unsigned int size, Addr addr,
        Request::Flags flags, uint64_t *res) override;

    void setStCondFailures(unsigned int sc_failures) override
    { thread.setStCondFailures(sc_failures); }

    unsigned int readStCondFailures() const override
    { return thread.readStCondFailures(); }

    // syscall emulation interfaces
    void syscall(int64_t callnum, Fault *fault) override
    { thread.syscall(callnum, fault); }

    ThreadContext *tcBase() override
    { return thread.getTC(); }

    // Alpha-specific interfaces
    Fault hwrei() override
    { return thread.hwrei(); }

    bool simPalCheck(int palFunc) override
    { return thread.simPalCheck(palFunc); }

    // ARM-specific interfaces
    bool readPredicate() override
    { return thread.readPredicate(); }

    void setPredicate(bool val) override
    { thread.setPredicate(val); }

    // X86-specific interfaces
    void demapPage(Addr vaddr, uint64_t asn) override
    { thread.demapPage(vaddr, asn); }

    void armMonitor(Addr address) override
    { panic("ARM monitor not implemented for MyCPU"); }
    
    bool mwait(PacketPtr pkt) override
    { panic("mwait not implemented for MyCPU"); }

    void mwaitAtomic(ThreadContext *tc) override
    { panic("mwaitAtomic not implemented for MyCPU"); }

    AddressMonitor *getAddrMonitor() override
    { panic("getAddrMonitor not implemented for MyCPU"); }

    // MIPS-specific interfaces
    #if THE_ISA == MIPS_ISA
    MiscReg readRegOtherThread(int regIdx, ThreadID tid = InvalidThreadID) override
    { panic("No support for multithreaded register access."); }

    void setRegOtherThread(int regIdx, MiscReg val,
        ThreadID tid = InvalidThreadID) override
    { panic("No support for multithreaded register access."); }
    #endif
};

#endif // __MYCPU_EXEC_CONTEXT_HH__


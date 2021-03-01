#ifndef __ARCH_POWER64_ISA_HH__
#define __ARCH_POWER64_ISA_HH__

#include "arch/generic/isa.hh"
#include "arch/power64/registers.hh"
#include "arch/power64/types.hh"
#include "base/logging.hh"
#include "cpu/reg_class.hh"
#include "sim/sim_object.hh"

struct Power64ISAParams;
class ThreadContext;
class Checkpoint;
class EventManager;

namespace Power64ISA
{

class ISA : public BaseISA
{
  protected:
    RegVal dummy;
    RegVal miscRegs[NumMiscRegs];

  public:
    typedef Power64ISAParams Params;

    void clear() {}

  public:
    RegVal
    readMiscRegNoEffect(int misc_reg) const
    {
        fatal("Power does not currently have any misc regs defined\n");
        return dummy;
    }

    RegVal
    readMiscReg(int misc_reg)
    {
        fatal("Power does not currently have any misc regs defined\n");
        return dummy;
    }

    void
    setMiscRegNoEffect(int misc_reg, RegVal val)
    {
        fatal("Power does not currently have any misc regs defined\n");
    }

    void
    setMiscReg(int misc_reg, RegVal val)
    {
        fatal("Power does not currently have any misc regs defined\n");
    }

    RegId flattenRegId(const RegId& regId) const { return regId; }

    int
    flattenIntIndex(int reg) const
    {
        return reg;
    }

    int
    flattenFloatIndex(int reg) const
    {
        return reg;
    }

    int
    flattenVecIndex(int reg) const
    {
        return reg;
    }

    int
    flattenVecElemIndex(int reg) const
    {
        return reg;
    }

    int
    flattenVecPredIndex(int reg) const
    {
        return reg;
    }

    // dummy
    int
    flattenCCIndex(int reg) const
    {
        return reg;
    }

    int
    flattenMiscIndex(int reg) const
    {
        return reg;
    }

    const Params *params() const;

    ISA(Params *p);
};

} // namespace Power64ISA

#endif // __ARCH_POWER64_ISA_HH__

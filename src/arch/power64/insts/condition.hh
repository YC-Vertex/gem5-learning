#ifndef __ARCH_POWER64_INSTS_CONDITION_HH__
#define __ARCH_POWER64_INSTS_CONDITION_HH__

#include "arch/power64/insts/static_inst.hh"
#include "base/cprintf.hh"

namespace Power64ISA
{

/**
 * Class for condition register logical operations.
 */
class CondLogicOp : public Power64StaticInst
{
  protected:

    uint32_t ba;
    uint32_t bb;
    uint32_t bt;

    /// Constructor
    CondLogicOp(const char *mnem, MachInst _machInst, OpClass __opClass)
      : Power64StaticInst(mnem, _machInst, __opClass),
        ba(machInst.ba),
        bb(machInst.bb),
        bt(machInst.bt)
    {
    }

    std::string generateDisassembly(
            Addr pc, const Loader::SymbolTable *symtab) const override;
};

/**
 * Class for condition register move operations.
 */
class CondMoveOp : public Power64StaticInst
{
  protected:

    uint32_t bf;
    uint32_t bfa;

    /// Constructor
    CondMoveOp(const char *mnem, MachInst _machInst, OpClass __opClass)
      : Power64StaticInst(mnem, _machInst, __opClass),
        bf(machInst.bf),
        bfa(machInst.bfa)
    {
    }

    std::string generateDisassembly(
            Addr pc, const Loader::SymbolTable *symtab) const override;
};

} // namespace Power64ISA

#endif //__ARCH_POWER64_INSTS_CONDITION_HH__

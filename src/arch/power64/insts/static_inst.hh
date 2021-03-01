#ifndef __ARCH_POWER64_INSTS_STATICINST_HH__
#define __ARCH_POWER64_INSTS_STATICINST_HH__

#include "base/trace.hh"
#include "cpu/static_inst.hh"

namespace Power64ISA
{

class Power64StaticInst : public StaticInst
{
  protected:

    // Constructor
    Power64StaticInst(const char *mnem, MachInst _machInst, OpClass __opClass)
        : StaticInst(mnem, _machInst, __opClass)
    {
    }

    // Insert a condition value into a CR (condition register) field
    inline uint32_t
    insertCRField(uint32_t cr, uint32_t bf, uint32_t value) const
    {
        uint32_t bits = value << ((7 - bf) * 4);
        uint32_t mask = ~(0xf << ((7 - bf) * 4));
        return (cr & mask) | bits;
    }

    /// Print a register name for disassembly given the unique
    /// dependence tag number (FP or int).
    void
    printReg(std::ostream &os, RegId reg) const;

    std::string generateDisassembly(
            Addr pc, const Loader::SymbolTable *symtab) const override;

    void
    advancePC(Power64ISA::PCState &pcState) const override
    {
        pcState.advance();
    }

    size_t
    asBytes(void *buf, size_t max_size) override
    {
        return simpleAsBytes(buf, max_size, machInst);
    }
};

} // namespace Power64ISA

#endif //__ARCH_POWER64_INSTS_STATICINST_HH__

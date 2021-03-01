#ifndef __ARCH_POWER64_MEM_HH__
#define __ARCH_POWER64_MEM_HH__

#include "arch/power64/insts/static_inst.hh"

namespace Power64ISA
{

/**
 * Base class for memory operations.
 */
class MemOp : public Power64StaticInst
{
  protected:

    /// Memory request flags.  See mem_req_base.hh.
    unsigned memAccessFlags;

    /// Constructor
    MemOp(const char *mnem, MachInst _machInst, OpClass __opClass)
      : Power64StaticInst(mnem, _machInst, __opClass),
        memAccessFlags(0)
    {
    }

    std::string generateDisassembly(
            Addr pc, const Loader::SymbolTable *symtab) const override;
};


/**
 * Class for memory operations with displacement.
 */
class MemDispOp : public MemOp
{
  protected:

    int16_t disp;

    /// Constructor
    MemDispOp(const char *mnem, MachInst _machInst, OpClass __opClass)
      : MemOp(mnem, _machInst, __opClass), disp(machInst.d)
    {
    }

    std::string generateDisassembly(
            Addr pc, const Loader::SymbolTable *symtab) const override;
};

} // namespace Power64ISA

#endif //__ARCH_POWER64_INSTS_MEM_HH__

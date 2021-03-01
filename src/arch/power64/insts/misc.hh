#ifndef __ARCH_POWER64_INSTS_MISC_HH__
#define __ARCH_POWER64_INSTS_MISC_HH__

#include "arch/power64/insts/static_inst.hh"

namespace Power64ISA
{

/**
 * Class for misc operations.
 */
class MiscOp : public Power64StaticInst
{
  protected:
    using Power64StaticInst::Power64StaticInst;

    std::string generateDisassembly(
            Addr pc, const Loader::SymbolTable *symtab) const override;
};

} // namespace Power64ISA

#endif //__ARCH_POWER64_INSTS_MISC_HH__

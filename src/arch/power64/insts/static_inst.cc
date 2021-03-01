#include "arch/power64/insts/static_inst.hh"

#include "cpu/reg_class.hh"

using namespace Power64ISA;

void
Power64StaticInst::printReg(std::ostream &os, RegId reg) const
{
    if (reg.isIntReg())
        ccprintf(os, "r%d", reg.index());
    else if (reg.isFloatReg())
        ccprintf(os, "f%d", reg.index());
    else if (reg.isMiscReg())
        switch (reg.index()) {
          case 0: ccprintf(os, "cr"); break;
          case 1: ccprintf(os, "xer"); break;
          case 2: ccprintf(os, "lr"); break;
          case 3: ccprintf(os, "ctr"); break;
          default: ccprintf(os, "unknown_reg");
            break;
        }
    else if (reg.isCCReg())
        panic("printReg: POWER does not implement CCRegClass\n");
}

std::string
Power64StaticInst::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;

    ccprintf(ss, "%-10s ", mnemonic);

    return ss.str();
}

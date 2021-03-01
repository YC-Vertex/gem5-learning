#include "arch/power64/insts/floating.hh"

using namespace Power64ISA;

std::string
FloatOp::generateDisassembly(Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;

    ccprintf(ss, "%-10s ", mnemonic);

    // Print the first destination only
    if (_numDestRegs > 0) {
        printReg(ss, _destRegIdx[0]);
    }

    // Print the (possibly) two source registers
    if (_numSrcRegs > 0) {
        if (_numDestRegs > 0) {
            ss << ", ";
        }
        printReg(ss, _srcRegIdx[0]);
        if (_numSrcRegs > 1) {
          ss << ", ";
          printReg(ss, _srcRegIdx[1]);
        }
    }

    return ss.str();
}

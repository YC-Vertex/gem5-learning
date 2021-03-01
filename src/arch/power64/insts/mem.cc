#include "arch/power64/insts/mem.hh"

#include "base/loader/symtab.hh"

using namespace Power64ISA;

std::string
MemOp::generateDisassembly(Addr pc, const Loader::SymbolTable *symtab) const
{
    return csprintf("%-10s", mnemonic);
}

std::string
MemDispOp::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;

    ccprintf(ss, "%-10s ", mnemonic);

    // Print the destination only for a load
    if (!flags[IsStore]) {
        if (_numDestRegs > 0) {

            // If the instruction updates the source register with the
            // EA, then this source register is placed in position 0,
            // therefore we print the last destination register.
            printReg(ss, _destRegIdx[_numDestRegs-1]);
        }
    }

    // Print the data register for a store
    else {
        printReg(ss, _srcRegIdx[1]);
    }

    // Print the displacement
    ss << ", " << (int32_t)disp;

    // Print the address register
    ss << "(";
    printReg(ss, _srcRegIdx[0]);
    ss << ")";

    return ss.str();
}

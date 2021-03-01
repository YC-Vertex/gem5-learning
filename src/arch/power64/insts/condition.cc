#include "arch/power64/insts/condition.hh"

using namespace Power64ISA;

std::string
CondLogicOp::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;

    ccprintf(ss, "%-10s ", mnemonic);

    // Format is <mnemonic> bt, ba, bb
    ss << bt << ", " << ba << ", " << bb;

    return ss.str();
}

std::string
CondMoveOp::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;

    ccprintf(ss, "%-10s ", mnemonic);

    // Format is <mnemonic> bf, bfa
    ss << bf << ", " << bfa;

    return ss.str();
}

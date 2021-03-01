#include "arch/power64/insts/branch.hh"

#include "base/loader/symtab.hh"
#include "cpu/thread_context.hh"

using namespace Power64ISA;

const std::string &
PCDependentDisassembly::disassemble(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    if (!cachedDisassembly ||
        pc != cachedPC || symtab != cachedSymtab)
    {
        if (cachedDisassembly)
            delete cachedDisassembly;

        cachedDisassembly =
            new std::string(generateDisassembly(pc, symtab));
        cachedPC = pc;
        cachedSymtab = symtab;
    }

    return *cachedDisassembly;
}

Power64ISA::PCState
BranchPCRel::branchTarget(const Power64ISA::PCState &pc) const
{
    return (uint32_t)(pc.pc() + disp);
}

std::string
BranchPCRel::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;

    ccprintf(ss, "%-10s ", mnemonic);

    Addr target = pc + disp;

    Loader::SymbolTable::const_iterator it;
    if (symtab && (it = symtab->find(target)) != symtab->end())
        ss << it->name;
    else
        ccprintf(ss, "%#x", target);

    return ss.str();
}

Power64ISA::PCState
BranchNonPCRel::branchTarget(const Power64ISA::PCState &pc) const
{
    return targetAddr;
}

std::string
BranchNonPCRel::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;

    ccprintf(ss, "%-10s ", mnemonic);

    Loader::SymbolTable::const_iterator it;
    if (symtab && (it = symtab->find(targetAddr)) != symtab->end())
        ss << it->name;
    else
        ccprintf(ss, "%#x", targetAddr);

    return ss.str();
}

Power64ISA::PCState
BranchPCRelCond::branchTarget(const Power64ISA::PCState &pc) const
{
    return (uint32_t)(pc.pc() + disp);
}

std::string
BranchPCRelCond::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;

    ccprintf(ss, "%-10s ", mnemonic);

    ss << bo << ", " << bi << ", ";

    Addr target = pc + disp;

    Loader::SymbolTable::const_iterator it;
    if (symtab && (it = symtab->find(target)) != symtab->end())
        ss << it->name;
    else
        ccprintf(ss, "%#x", target);

    return ss.str();
}

Power64ISA::PCState
BranchNonPCRelCond::branchTarget(const Power64ISA::PCState &pc) const
{
    return targetAddr;
}

std::string
BranchNonPCRelCond::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;

    ccprintf(ss, "%-10s ", mnemonic);

    ss << bo << ", " << bi << ", ";

    Loader::SymbolTable::const_iterator it;
    if (symtab && (it = symtab->find(targetAddr)) != symtab->end())
        ss << it->name;
    else
        ccprintf(ss, "%#x", targetAddr);

    return ss.str();
}

Power64ISA::PCState
BranchRegCond::branchTarget(ThreadContext *tc) const
{
    uint32_t regVal = tc->readIntReg(_srcRegIdx[_numSrcRegs - 1].index());
    return regVal & 0xfffffffc;
}

std::string
BranchRegCond::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    std::stringstream ss;

    ccprintf(ss, "%-10s ", mnemonic);

    ss << bo << ", " << bi << ", ";

    return ss.str();
}

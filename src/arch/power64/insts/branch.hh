#ifndef __ARCH_POWER64_INSTS_BRANCH_HH__
#define __ARCH_POWER64_INSTS_BRANCH_HH__

#include "arch/power64/insts/static_inst.hh"

namespace Power64ISA
{

/**
 * Base class for instructions whose disassembly is not purely a
 * function of the machine instruction (i.e., it depends on the
 * PC).  This class overrides the disassemble() method to check
 * the PC and symbol table values before re-using a cached
 * disassembly string.  This is necessary for branches and jumps,
 * where the disassembly string includes the target address (which
 * may depend on the PC and/or symbol table).
 */
class PCDependentDisassembly : public Power64StaticInst
{
  protected:
    /// Cached program counter from last disassembly
    mutable Addr cachedPC;
    /// Cached symbol table pointer from last disassembly
    mutable const Loader::SymbolTable *cachedSymtab;

    /// Constructor
    PCDependentDisassembly(const char *mnem, ExtMachInst _machInst,
                           OpClass __opClass)
        : Power64StaticInst(mnem, _machInst, __opClass),
          cachedPC(0), cachedSymtab(0)
    {
    }

    const std::string &
    disassemble(Addr pc, const Loader::SymbolTable *symtab) const;
};

/**
 * Base class for unconditional, PC-relative branches.
 */
class BranchPCRel : public PCDependentDisassembly
{
  protected:

    /// Displacement
    uint32_t disp;

    /// Constructor.
    BranchPCRel(const char *mnem, MachInst _machInst, OpClass __opClass)
        : PCDependentDisassembly(mnem, _machInst, __opClass),
          disp(machInst.li << 2)
    {
        // If bit 26 is 1 then sign extend
        if (disp & 0x2000000) {
            disp |= 0xfc000000;
        }
    }

    Power64ISA::PCState branchTarget(const Power64ISA::PCState &pc) const override;

    /// Explicitly import the otherwise hidden branchTarget
    using StaticInst::branchTarget;

    std::string generateDisassembly(
            Addr pc, const Loader::SymbolTable *symtab) const override;
};

/**
 * Base class for unconditional, non PC-relative branches.
 */
class BranchNonPCRel : public PCDependentDisassembly
{
  protected:

    /// Target address
    uint32_t targetAddr;

    /// Constructor.
    BranchNonPCRel(const char *mnem, MachInst _machInst, OpClass __opClass)
        : PCDependentDisassembly(mnem, _machInst, __opClass),
          targetAddr(machInst.li << 2)
    {
        // If bit 26 is 1 then sign extend
        if (targetAddr & 0x2000000) {
            targetAddr |= 0xfc000000;
        }
    }

    Power64ISA::PCState branchTarget(const Power64ISA::PCState &pc) const override;

    /// Explicitly import the otherwise hidden branchTarget
    using StaticInst::branchTarget;

    std::string generateDisassembly(
            Addr pc, const Loader::SymbolTable *symtab) const override;
};

/**
 * Base class for conditional branches.
 */
class BranchCond : public PCDependentDisassembly
{
  protected:

    /// Fields needed for conditions
    uint32_t bo;
    uint32_t bi;

    /// Constructor.
    BranchCond(const char *mnem, MachInst _machInst, OpClass __opClass)
        : PCDependentDisassembly(mnem, _machInst, __opClass),
          bo(machInst.bo),
          bi(machInst.bi)
    {
    }

    inline bool
    ctrOk(uint32_t& ctr) const
    {
        bool ctr_ok;
        if (bo & 4) {
            ctr_ok = true;
        } else {
            ctr--;
            if (ctr != 0) {
                ctr_ok = ((bo & 2) == 0);
            } else {
                ctr_ok = ((bo & 2) != 0);
            }
        }
        return ctr_ok;
    }

    inline bool
    condOk(uint32_t cr) const
    {
        bool cond_ok;
        if (bo & 16) {
            cond_ok = true;
        } else {
            cond_ok = (((cr >> (31 - bi)) & 1) == ((bo >> 3) & 1));
        }
        return cond_ok;
    }
};

/**
 * Base class for conditional, PC-relative branches.
 */
class BranchPCRelCond : public BranchCond
{
  protected:

    /// Displacement
    uint32_t disp;

    /// Constructor.
    BranchPCRelCond(const char *mnem, MachInst _machInst, OpClass __opClass)
        : BranchCond(mnem, _machInst, __opClass),
          disp(machInst.bd << 2)
    {
        // If bit 16 is 1 then sign extend
        if (disp & 0x8000) {
            disp |= 0xffff0000;
        }
    }

    Power64ISA::PCState branchTarget(const Power64ISA::PCState &pc) const override;

    /// Explicitly import the otherwise hidden branchTarget
    using StaticInst::branchTarget;

    std::string generateDisassembly(
            Addr pc, const Loader::SymbolTable *symtab) const override;
};

/**
 * Base class for conditional, non PC-relative branches.
 */
class BranchNonPCRelCond : public BranchCond
{
  protected:

    /// Target address
    uint32_t targetAddr;

    /// Constructor.
    BranchNonPCRelCond(const char *mnem, MachInst _machInst, OpClass __opClass)
        : BranchCond(mnem, _machInst, __opClass),
          targetAddr(machInst.bd << 2)
    {
        // If bit 16 is 1 then sign extend
        if (targetAddr & 0x8000) {
            targetAddr |= 0xffff0000;
        }
    }

    Power64ISA::PCState branchTarget(const Power64ISA::PCState &pc) const override;

    /// Explicitly import the otherwise hidden branchTarget
    using StaticInst::branchTarget;

    std::string generateDisassembly(
            Addr pc, const Loader::SymbolTable *symtab) const override;
};

/**
 * Base class for conditional, register-based branches
 */
class BranchRegCond : public BranchCond
{
  protected:

    /// Constructor.
    BranchRegCond(const char *mnem, MachInst _machInst, OpClass __opClass)
        : BranchCond(mnem, _machInst, __opClass)
    {
    }

    Power64ISA::PCState branchTarget(ThreadContext *tc) const override;

    /// Explicitly import the otherwise hidden branchTarget
    using StaticInst::branchTarget;

    std::string generateDisassembly(
            Addr pc, const Loader::SymbolTable *symtab) const override;
};

} // namespace Power64ISA

#endif //__ARCH_POWER64_INSTS_BRANCH_HH__

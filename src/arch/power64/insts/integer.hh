#ifndef __ARCH_POWER64_INSTS_INTEGER_HH__
#define __ARCH_POWER64_INSTS_INTEGER_HH__

#include "arch/power64/insts/static_inst.hh"
#include "base/bitfield.hh"
#include "base/cprintf.hh"

namespace Power64ISA
{

/**
 * We provide a base class for integer operations and then inherit for
 * several other classes. These specialise for instructions using immediate
 * values and also rotate instructions. We also need to have versions that
 * consider the Rc and OE bits.
 */

/**
 * Base class for integer operations.
 */
class IntOp : public Power64StaticInst
{
  protected:

    bool rcSet;
    bool oeSet;

    // Needed for srawi only
    uint32_t sh;

    /// Constructor
    IntOp(const char *mnem, MachInst _machInst, OpClass __opClass)
      : Power64StaticInst(mnem, _machInst, __opClass),
        rcSet(false), oeSet(false)
    {
    }

    /* Compute the CR (condition register) field using signed comparison */
    inline uint32_t
    makeCRField(int32_t a, int32_t b, uint32_t xerSO) const
    {
        uint32_t c = xerSO;

        /* We've pre-shifted the immediate values here */
        if (a < b)      { c += 0x8; }
        else if (a > b) { c += 0x4; }
        else            { c += 0x2; }
        return c;
    }

    /* Compute the CR (condition register) field using unsigned comparison */
    inline uint32_t
    makeCRField(uint32_t a, uint32_t b, uint32_t xerSO) const
    {
        uint32_t c = xerSO;

        /* We've pre-shifted the immediate values here */
        if (a < b)      { c += 0x8; }
        else if (a > b) { c += 0x4; }
        else            { c += 0x2; }
        return c;
    }

    std::string generateDisassembly(
            Addr pc, const Loader::SymbolTable *symtab) const override;
};


/**
 * Class for integer immediate (signed and unsigned) operations.
 */
class IntImmOp : public IntOp
{
  protected:

    int32_t imm;
    uint32_t uimm;

    /// Constructor
    IntImmOp(const char *mnem, MachInst _machInst, OpClass __opClass)
      : IntOp(mnem, _machInst, __opClass),
        imm(sext<16>(machInst.si)),
        uimm(machInst.si)
    {
    }

    std::string generateDisassembly(
            Addr pc, const Loader::SymbolTable *symtab) const override;
};


/**
 * Class for integer operations with a shift.
 */
class IntShiftOp : public IntOp
{
  protected:

    uint32_t sh;

    /// Constructor
    IntShiftOp(const char *mnem, MachInst _machInst, OpClass __opClass)
      : IntOp(mnem, _machInst, __opClass),
        sh(machInst.sh)
    {
    }

    std::string generateDisassembly(
            Addr pc, const Loader::SymbolTable *symtab) const override;
};


/**
 * Class for integer rotate operations.
 */
class IntRotateOp : public IntShiftOp
{
  protected:

    uint32_t mb;
    uint32_t me;
    uint32_t fullMask;

    /// Constructor
    IntRotateOp(const char *mnem, MachInst _machInst, OpClass __opClass)
      : IntShiftOp(mnem, _machInst, __opClass),
        mb(machInst.mb),
        me(machInst.me)
    {
        if (me >= mb) {
            fullMask = mask(31 - mb, 31 - me);
        } else {
            fullMask = ~mask(31 - (me + 1), 31 - (mb - 1));
        }
    }

    uint32_t
    rotateValue(uint32_t rs, uint32_t shift) const
    {
        uint32_t n = shift & 31;
        return (rs << n) | (rs >> (32 - n));
    }

    std::string generateDisassembly(
            Addr pc, const Loader::SymbolTable *symtab) const override;
};

} // namespace Power64ISA

#endif //__ARCH_POWER64_INSTS_INTEGER_HH__

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
 * Class for double-word integer (64-bit) operations with a shift.
 */
class IntShiftOp_64 : public IntOp
{
  protected:

    uint64_t sh;

    /// Constructor
    IntShiftOp_64(const char *mnem, MachInst _machInst, OpClass __opClass)
      : IntOp(mnem, _machInst, __opClass),
        sh((machInst.sh_ext << 5) | machInst.sh)
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

    uint64_t mb;
    uint64_t me;
    uint64_t fullMask;

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

    uint64_t
    rotateValue(uint64_t rs, uint64_t shift) const
    {
        uint64_t n = shift & 31;
        uint64_t mrs = mbits(rs, 31, 0);  // mask
        uint64_t crs = mrs | (mrs << 32); // concatenate
        return (crs << n) | (crs >> (64 - n));
    }

    std::string generateDisassembly(
            Addr pc, const Loader::SymbolTable *symtab) const override;
};


/**
 * Class for double-word integer (64-bit) rotate operations.
 */
class IntRotateOp_64 : public IntShiftOp_64
{
  protected:

    uint64_t mb_me;

    /// Constructor
    IntRotateOp_64(const char *mnem, MachInst _machInst, OpClass __opClass)
      : IntShiftOp_64(mnem, _machInst, __opClass),
        mb_me(machInst.mb_me)
    {
    }

    uint64_t
    genMask(uint64_t mb, uint64_t me) const
    {
        uint64_t fullMask;
        if (me >= mb) {
            fullMask = mask(63 - mb, 63 - me);
        } else {
            fullMask = ~mask(63 - (me + 1), 63 - (mb - 1));
        }
        return fullMask;
    }

    uint64_t
    rotateValue(uint64_t rs, uint64_t shift) const
    {
        uint64_t n = shift & 63;
        return (rs << n) | (rs >> (64 - n));
    }

    std::string generateDisassembly(
            Addr pc, const Loader::SymbolTable *symtab) const override;
};

} // namespace Power64ISA

#endif //__ARCH_POWER64_INSTS_INTEGER_HH__

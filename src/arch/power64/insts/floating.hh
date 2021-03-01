#ifndef __ARCH_POWER64_INSTS_FLOATING_HH__
#define __ARCH_POWER64_INSTS_FLOATING_HH__

#include "arch/power64/insts/static_inst.hh"
#include "base/bitfield.hh"
#include "base/cprintf.hh"

namespace Power64ISA
{

/**
 * Base class for floating point operations.
 */
class FloatOp : public Power64StaticInst
{
  protected:

    bool rcSet;

    /// Constructor
    FloatOp(const char *mnem, MachInst _machInst, OpClass __opClass)
      : Power64StaticInst(mnem, _machInst, __opClass)
    {
    }

    // Test for NaN (maximum biased exponent & non-zero fraction)
    inline bool
    isNan(uint32_t val_bits) const
    {
        return ((bits(val_bits, 30, 23) == 0xFF) && bits(val_bits, 22, 0));
    }

    inline bool
    isNan(uint64_t val_bits) const
    {
        return ((bits(val_bits, 62, 52) == 0x7FF) && bits(val_bits, 51, 0));
    }

    inline bool
    isNan(float val) const
    {
        void *val_ptr = &val;
        uint32_t val_bits = *(uint32_t *) val_ptr;
        return isNan(val_bits);
    }

    inline bool
    isNan(double val) const
    {
        void *val_ptr = &val;
        uint64_t val_bits = *(uint64_t *) val_ptr;
        return isNan(val_bits);
    }

    // Test for SNaN (NaN with high order bit of fraction set to 0)
    inline bool
    isSnan(uint32_t val_bits) const
    {
        return ((bits(val_bits, 30, 22) == 0x1FE) && bits(val_bits, 22, 0));
    }

    // Test for QNaN (NaN with high order bit of fraction set to 1)
    inline bool
    isQnan(uint32_t val_bits) const
    {
        return (bits(val_bits, 30, 22) == 0x1FF);
    }

    // Test for infinity (maximum biased exponent and zero fraction)
    inline bool
    isInfinity(uint32_t val_bits) const
    {
        return ((bits(val_bits, 30, 23) == 0xFF) && !bits(val_bits, 22, 0));
    }

    // Test for normalized numbers (biased exponent in the range 1 to 254)
    inline bool
    isNormalized(uint32_t val_bits) const
    {
        return ((bits(val_bits, 30, 23) != 0xFF) && bits(val_bits, 22, 0));
    }

    // Test for denormalized numbers (biased exponent of zero and
    // non-zero fraction)
    inline bool
    isDenormalized(uint32_t val_bits) const
    {
        return (!bits(val_bits, 30, 23) && bits(val_bits, 22, 0));
    }

    // Test for zero (biased exponent of zero and fraction of zero)
    inline bool
    isZero(uint32_t val_bits) const
    {
        return (!bits(val_bits, 30, 23) && !bits(val_bits, 22, 0));
    }

    // Test for negative
    inline bool
    isNegative(uint32_t val_bits) const
    {
        return (bits(val_bits, 31));
    }

    // Compute the CR field
    inline uint32_t
    makeCRField(double a, double b) const
    {
        uint32_t c = 0;
        if (isNan(a) || isNan(b)) { c = 0x1; }
        else if (a < b)           { c = 0x8; }
        else if (a > b)           { c = 0x4; }
        else                      { c = 0x2; }
        return c;
    }

    std::string generateDisassembly(
            Addr pc, const Loader::SymbolTable *symtab) const override;
};

} // namespace Power64ISA

#endif //__ARCH_POWER64_INSTS_FLOATING_HH__

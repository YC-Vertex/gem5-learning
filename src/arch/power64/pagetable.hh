#ifndef __ARCH_POWER64_PAGETABLE_H__
#define __ARCH_POWER64_PAGETABLE_H__

#include "arch/power64/isa_traits.hh"
#include "arch/power64/utility.hh"

namespace Power64ISA
{

// ITB/DTB page table entry
struct PTE
{
    // What parts of the VAddr (from bits 28..11) should be used in
    // translation (includes Mask and MaskX from PageMask)
    Addr Mask;

    // Virtual Page Number (/2) (Includes VPN2 + VPN2X .. bits 31..11
    // from EntryHi)
    Addr VPN;

    // Address Space ID (8 bits) // Lower 8 bits of EntryHi
    uint8_t asid;

    // Global Bit - Obtained by an *AND* of EntryLo0 and EntryLo1 G bit
    bool G;

    /* Contents of Entry Lo0 */
    Addr PFN0; // Physical Frame Number - Even
    bool D0;   // Even entry Dirty Bit
    bool V0;   // Even entry Valid Bit
    uint8_t C0; // Cache Coherency Bits - Even

    /* Contents of Entry Lo1 */
    Addr PFN1; // Physical Frame Number - Odd
    bool D1;   // Odd entry Dirty Bit
    bool V1;   // Odd entry Valid Bit
    uint8_t C1; // Cache Coherency Bits (3 bits)

    // The next few variables are put in as optimizations to reduce TLB
    // lookup overheads. For a given Mask, what is the address shift amount
    // and what is the OffsetMask
    int AddrShiftAmount;
    int OffsetMask;

    bool
    Valid()
    {
        return (V0 | V1);
    };

    void serialize(CheckpointOut &cp) const;
    void unserialize(CheckpointIn &cp);
};

} // namespace Power64ISA

#endif // __ARCH_POWER64_PAGETABLE_H__


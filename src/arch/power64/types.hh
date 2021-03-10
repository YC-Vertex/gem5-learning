#ifndef __ARCH_POWER64_TYPES_HH__
#define __ARCH_POWER64_TYPES_HH__

#include "arch/generic/types.hh"
#include "base/bitunion.hh"
#include "base/types.hh"

namespace Power64ISA
{

typedef uint32_t MachInst;

BitUnion32(ExtMachInst)

    // Registers
    Bitfield<25, 21> rs;
    Bitfield<20, 16> ra;

    // Shifts and masks
    Bitfield<15, 11> sh;
    Bitfield<1>      sh_ext; // extended, for double-word rotate
    Bitfield<10,  6> mb;
    Bitfield< 5,  1> me;
    Bitfield<10,  5> mb_me;  // for double-word rotate

    // Immediate fields
    Bitfield<15,  0> si;
    Bitfield<15,  0> d;
    Bitfield<15,  2> ds;

    // Special purpose register identifier
    Bitfield<20, 11> spr;
    Bitfield<25,  2> li;
    Bitfield<1>      aa;
    Bitfield<25, 23> bf;
    Bitfield<15,  2> bd;
    Bitfield<25, 21> bo;
    Bitfield<20, 16> bi;
    Bitfield<20, 18> bfa;

    // Record bits
    Bitfield<0>      rc31;
    Bitfield<10>     oe;

    // Condition register fields
    Bitfield<25, 21> bt;
    Bitfield<20, 16> ba;
    Bitfield<15, 11> bb;

    // FXM field for mtcrf instruction
    Bitfield<19, 12> fxm;
EndBitUnion(ExtMachInst)

typedef GenericISA::SimplePCState<MachInst> PCState;

// typedef uint64_t LargestRead;
// // Need to use 64 bits to make sure that read requests get handled properly

// typedef int RegContextParam;
// typedef int RegContextVal;

} // Power64ISA namespace

namespace std {

template<>
struct hash<Power64ISA::ExtMachInst> : public hash<uint32_t> {
    size_t operator()(const Power64ISA::ExtMachInst &emi) const {
        return hash<uint32_t>::operator()((uint32_t)emi);
    };
};

}

#endif // __ARCH_POWER64_TYPES_HH__

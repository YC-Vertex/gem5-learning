#ifndef __ARCH_POWER64_MISCREGS_HH__
#define __ARCH_POWER64_MISCREGS_HH__

#include "base/bitunion.hh"

namespace Power64ISA
{

enum MiscRegIndex {
    NUM_MISCREGS = 0
};

const char * const miscRegName[NUM_MISCREGS] = {
};

BitUnion32(Cr)
    SubBitUnion(cr0, 31, 28)
        Bitfield<31> lt;
        Bitfield<30> gt;
        Bitfield<29> eq;
        Bitfield<28> so;
    EndSubBitUnion(cr0)
    Bitfield<27,24> cr1;
EndBitUnion(Cr)

BitUnion32(Xer)
    Bitfield<31> so;
    Bitfield<30> ov;
    Bitfield<29> ca;
EndBitUnion(Xer)

BitUnion32(Fpscr)
    Bitfield<31> fx;
    Bitfield<30> fex;
    Bitfield<29> vx;
    Bitfield<28> ox;
    Bitfield<27> ux;
    Bitfield<26> zx;
    Bitfield<25> xx;
    Bitfield<24> vxsnan;
    Bitfield<23> vxisi;
    Bitfield<22> vxidi;
    Bitfield<21> vxzdz;
    Bitfield<20> vximz;
    Bitfield<19> vxvc;
    Bitfield<18> fr;
    Bitfield<17> fi;
    SubBitUnion(fprf, 16, 12)
        Bitfield<16> c;
        SubBitUnion(fpcc, 15, 12)
            Bitfield<15> fl;
            Bitfield<14> fg;
            Bitfield<13> fe;
            Bitfield<12> fu;
        EndSubBitUnion(fpcc)
    EndSubBitUnion(fprf)
    Bitfield<10> vxsqrt;
    Bitfield<9> vxcvi;
    Bitfield<8> ve;
    Bitfield<7> oe;
    Bitfield<6> ue;
    Bitfield<5> ze;
    Bitfield<4> xe;
    Bitfield<3> ni;
    Bitfield<2,1> rn;
EndBitUnion(Fpscr)

} // namespace Power64ISA

#endif // __ARCH_POWER64_MISCREGS_HH__

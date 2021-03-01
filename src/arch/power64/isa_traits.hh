#ifndef __ARCH_POWER64_ISA_TRAITS_HH__
#define __ARCH_POWER64_ISA_TRAITS_HH__

#include "base/types.hh"

namespace Power64ISA
{

const ByteOrder GuestByteOrder = ByteOrder::big;

const Addr PageShift = 12;
const Addr PageBytes = ULL(1) << PageShift;

} // namespace Power64ISA

#endif // __ARCH_POWER64_ISA_TRAITS_HH__

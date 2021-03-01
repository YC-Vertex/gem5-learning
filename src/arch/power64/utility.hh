#ifndef __ARCH_POWER64_UTILITY_HH__
#define __ARCH_POWER64_UTILITY_HH__

#include "base/types.hh"
#include "cpu/static_inst.hh"
#include "cpu/thread_context.hh"

namespace Power64ISA {

inline PCState
buildRetPC(const PCState &curPC, const PCState &callPC)
{
    PCState retPC = callPC;
    retPC.advance();
    return retPC;
}

void copyRegs(ThreadContext *src, ThreadContext *dest);

static inline void
copyMiscRegs(ThreadContext *src, ThreadContext *dest)
{
}

inline void
advancePC(PCState &pc, const StaticInstPtr &inst)
{
    pc.advance();
}

uint64_t getArgument(ThreadContext *tc, int &number, uint16_t size, bool fp);

static inline bool
inUserMode(ThreadContext *tc)
{
    return 0;
}

inline uint64_t
getExecutingAsid(ThreadContext *tc)
{
    return 0;
}

} // namespace Power64ISA


#endif // __ARCH_POWER64_UTILITY_HH__

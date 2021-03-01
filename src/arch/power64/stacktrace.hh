#ifndef __ARCH_POWER64_STACKTRACE_HH__
#define __ARCH_POWER64_STACKTRACE_HH__

#include "base/logging.hh"
#include "cpu/profile.hh"

namespace Power64ISA
{

class StackTrace : public BaseStackTrace
{
  protected:
    void
    trace(ThreadContext *tc, bool is_call) override
    {
        panic("StackTrace::trace not implemented.");
    }
};

} // namespace Power64ISA

#endif // __ARCH_POWER64_STACKTRACE_HH__

#ifndef __ARCH_POWER64_INTERRUPT_HH__
#define __ARCH_POWER64_INTERRUPT_HH__

#include "arch/generic/interrupts.hh"
#include "base/logging.hh"
#include "params/Power64Interrupts.hh"

class BaseCPU;
class ThreadContext;

namespace Power64ISA {

class Interrupts : public BaseInterrupts
{
  public:
    typedef Power64InterruptsParams Params;

    const Params *
    params() const
    {
        return dynamic_cast<const Params *>(_params);
    }

    Interrupts(Params *p) : BaseInterrupts(p) {}

    void
    post(int int_num, int index)
    {
        panic("Interrupts::post not implemented.\n");
    }

    void
    clear(int int_num, int index)
    {
        panic("Interrupts::clear not implemented.\n");
    }

    void
    clearAll()
    {
        panic("Interrupts::clearAll not implemented.\n");
    }

    bool
    checkInterrupts() const
    {
        panic("Interrupts::checkInterrupts not implemented.\n");
    }

    Fault
    getInterrupt()
    {
        assert(checkInterrupts());
        panic("Interrupts::getInterrupt not implemented.\n");
    }

    void
    updateIntrInfo()
    {
        panic("Interrupts::updateIntrInfo not implemented.\n");
    }
};

} // namespace Power64ISA

#endif // __ARCH_POWER64_INTERRUPT_HH__


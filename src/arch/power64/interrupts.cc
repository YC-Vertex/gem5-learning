#include "arch/power64/interrupts.hh"

Power64ISA::Interrupts *
Power64InterruptsParams::create()
{
    return new Power64ISA::Interrupts(this);
}

#include "arch/power64/isa.hh"

#include "params/Power64ISA.hh"

namespace Power64ISA
{

ISA::ISA(Params *p) : BaseISA(p)
{
    clear();
}

const Power64ISAParams *
ISA::params() const
{
    return dynamic_cast<const Params *>(_params);
}

}

Power64ISA::ISA *
Power64ISAParams::create()
{
    return new Power64ISA::ISA(this);
}


#include "arch/power64/utility.hh"

#include "base/logging.hh"

namespace Power64ISA {

void
copyRegs(ThreadContext *src, ThreadContext *dest)
{
    // First loop through the integer registers.
    for (int i = 0; i < NumIntRegs; ++i)
        dest->setIntReg(i, src->readIntReg(i));

    // Then loop through the floating point registers.
    for (int i = 0; i < NumFloatRegs; ++i)
        dest->setFloatReg(i, src->readFloatReg(i));

    // Would need to add condition-code regs if implemented
    assert(NumCCRegs == 0);

    // Copy misc. registers
    copyMiscRegs(src, dest);

    // Lastly copy PC/NPC
    dest->pcState(src->pcState());
}

uint64_t
getArgument(ThreadContext *tc, int &number, uint16_t size, bool fp)
{
    panic("getArgument not implemented for POWER.\n");
    return 0;
}

} // namespace Power64ISA

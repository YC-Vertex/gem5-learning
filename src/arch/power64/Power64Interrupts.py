from m5.objects.BaseInterrupts import BaseInterrupts

class Power64Interrupts(BaseInterrupts):
    type = 'Power64Interrupts'
    cxx_class = 'Power64ISA::Interrupts'
    cxx_header = 'arch/power64/interrupts.hh'

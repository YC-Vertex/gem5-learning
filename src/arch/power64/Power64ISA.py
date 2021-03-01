from m5.objects.BaseISA import BaseISA

class Power64ISA(BaseISA):
    type = 'Power64ISA'
    cxx_class = 'Power64ISA::ISA'
    cxx_header = "arch/power64/isa.hh"

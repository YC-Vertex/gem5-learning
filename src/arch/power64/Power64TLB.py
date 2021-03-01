from m5.SimObject import SimObject
from m5.params import *

from m5.objects.BaseTLB import BaseTLB

class Power64TLB(BaseTLB):
    type = 'Power64TLB'
    cxx_class = 'Power64ISA::TLB'
    cxx_header = 'arch/power64/tlb.hh'
    size = Param.Int(64, "TLB size")

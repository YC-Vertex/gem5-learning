# transfer paramters to C++ objects
from m5.params import *
# ?
from m5.proxy import *
# import base classes
from m5.SimObject import SimObject

class HwMemobj(SimObject):
    type = 'HwMemobj'
    cxx_header = 'hello_world/hw_memobj.hh'

    inst_port = ResponsePort('CPU side port, receives instructions')
    data_port = ResponsePort('CPU side port, receives data')
    mem_side = RequestPort('Memory side port, sends requests')


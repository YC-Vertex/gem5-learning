from m5.params import *
from m5.SimObject import SimObject

class HelloWorldObject(SimObject):
    type = 'HelloWorldObject'
    cxx_header = 'hello_world/hello_world_object.hh'

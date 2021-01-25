from BaseCPU import BaseCPU

class MyCPU(BaseCPU):
    type = 'MyCPU'
    cxx_header = 'cpu/mycpu/cpu.hh'

    @classmethod
    def memory_mode(cls):
        return 'timing'


# import all gem5's objects
import m5
from m5.objects import * # imports all SimObjects

# instanitate a system
# "System" is a Python class wrapper for the System C++ SimObject
system = System()

# Initialize a clock and voltage domain
# "clk_domain" is a parameter of the System SimObject
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = '1GHz'
system.clk_domain.voltage_domain = VoltageDomain()

# Set up the memory system
system.mem_mode = 'timing'
system.mem_ranges = [AddrRange('1GB')]

# CPU
system.cpu = TimingSimpleCPU()

# Memory bus
system.membus = SystemXBar()

# Hook up CPU
system.cpu.icache_port = system.membus.slave
system.cpu.dcache_port = system.membus.slave

# BS
system.cpu.createInterruptController()
system.cpu.interrupts[0].pio = system.membus.master
system.cpu.interrupts[0].int_master = system.membus.slave
system.cpu.interrupts[0].int_slave = system.membus.master

system.system_port = system.membus.slave

# Memory controller
system.mem_ctrl = MemCtrl()
system.mem_ctrl.dram = DDR3_1600_8x8()
system.mem_ctrl.dram.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.master

# Tell the system what to do
process = Process()
process.cmd = ['tests/test-progs/sive_of_eratosthenes/sive_of_eratosthenes']
system.cpu.workload = process
system.cpu.createThreads()

# Create a root object
root = Root(full_system = False, system = system)

# Instantiate all of the C++ objects
m5.instantiate()

# Run
print('Beginning simulation!')
exit_event = m5.simulate()
print('Exiting @ tick %i because %s' % (m5.curTick(), exit_event.getCause()))

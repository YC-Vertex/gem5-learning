#ifndef __ARCH_POWER64_FAULTS_HH__
#define __ARCH_POWER64_FAULTS_HH__

#include "sim/faults.hh"

namespace Power64ISA
{

class Power64Fault : public FaultBase
{
  protected:
    FaultName _name;

    Power64Fault(FaultName name)
        : _name(name)
    {
    }

    FaultName
    name() const
    {
        return _name;
    }
};


class UnimplementedOpcodeFault : public Power64Fault
{
  public:
    UnimplementedOpcodeFault()
        : Power64Fault("Unimplemented Opcode")
    {
    }
};


class MachineCheckFault : public Power64Fault
{
  public:
    MachineCheckFault()
        : Power64Fault("Machine Check")
    {
    }
};


class AlignmentFault : public Power64Fault
{
  public:
    AlignmentFault()
        : Power64Fault("Alignment")
    {
    }
};

} // namespace Power64ISA

#endif // __ARCH_POWER64_FAULTS_HH__

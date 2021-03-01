#ifndef __POWER64_LINUX_PROCESS_HH__
#define __POWER64_LINUX_PROCESS_HH__

#include "arch/power64/process.hh"

#include "sim/syscall_desc.hh"

/// A process with emulated PPC/Linux syscalls.
class Power64LinuxProcess : public Power64Process
{
  public:
    Power64LinuxProcess(ProcessParams * params, ::Loader::ObjectFile *objFile);

    void initState() override;

    void syscall(ThreadContext *tc) override;

    /// Syscall descriptors, indexed by call number.
    static SyscallDescTable<SyscallABI> syscallDescs;
};

#endif // __POWER64_LINUX_PROCESS_HH__

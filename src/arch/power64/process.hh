#ifndef __POWER64_PROCESS_HH__
#define __POWER64_PROCESS_HH__

#include <string>
#include <vector>

#include "mem/page_table.hh"
#include "sim/process.hh"
#include "sim/syscall_abi.hh"

namespace Loader
{
class ObjectFile;
} // namespace Loader;

class Power64Process : public Process
{
  protected:
    Power64Process(ProcessParams * params, ::Loader::ObjectFile *objFile);

    void initState() override;

  public:
    void argsInit(int intSize, int pageSize);

    struct SyscallABI : public GenericSyscallABI64
    {
        static const std::vector<int> ArgumentRegs;
    };
};

namespace GuestABI
{

template <>
struct Result<Power64Process::SyscallABI, SyscallReturn>
{
    static void
    store(ThreadContext *tc, const SyscallReturn &ret)
    {
        if (ret.suppressed() || ret.needsRetry())
            return;

        Power64ISA::Cr cr = tc->readIntReg(Power64ISA::INTREG_CR);
        if (ret.successful()) {
            cr.cr0.so = 0;
        } else {
            cr.cr0.so = 1;
        }
        tc->setIntReg(Power64ISA::INTREG_CR, cr);
        tc->setIntReg(Power64ISA::ReturnValueReg, ret.encodedValue());
    }
};

} // namespace GuestABI

#endif // __POWER64_PROCESS_HH__


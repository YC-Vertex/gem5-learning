#ifndef __ARCH_POWER64_REMOTE_GDB_HH__
#define __ARCH_POWER64_REMOTE_GDB_HH__

#include "arch/power64/registers.hh"
#include "arch/power64/remote_gdb.hh"
#include "base/remote_gdb.hh"

namespace Power64ISA
{


class RemoteGDB : public BaseRemoteGDB
{
  protected:
    bool acc(Addr addr, size_t len);

    class PowerGdbRegCache : public BaseGdbRegCache
    {
      using BaseGdbRegCache::BaseGdbRegCache;
      private:
        struct {
            uint32_t gpr[NumIntArchRegs];
            uint64_t fpr[NumFloatArchRegs];
            uint32_t pc;
            uint32_t msr;
            uint32_t cr;
            uint32_t lr;
            uint32_t ctr;
            uint32_t xer;
        } r;
      public:
        char *data() const { return (char *)&r; }
        size_t size() const { return sizeof(r); }
        void getRegs(ThreadContext*);
        void setRegs(ThreadContext*) const;
        const std::string
        name() const
        {
            return gdb->name() + ".PowerGdbRegCache";
        }
    };

    PowerGdbRegCache regCache;

  public:
    RemoteGDB(System *_system, ThreadContext *tc, int _port);
    BaseGdbRegCache *gdbRegs();
    std::vector<std::string>
    availableFeatures() const
    {
        return {"qXfer:features:read+"};
    };
    bool getXferFeaturesRead(const std::string &annex, std::string &output);
};

} // namespace Power64ISA

#endif /* __ARCH_POWER64_REMOTE_GDB_H__ */

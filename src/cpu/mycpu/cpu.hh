#ifndef __CPU_MYCPU_CPU_HH__
#define __CPU_MYCPU_CPU_HH__

#include <vector>

#include "config/the_isa.hh"
#include "cpu/base.hh"
#include "cpu/mycpu/memory_request.hh"
#include "cpu/simple_thread.hh"
#include "params/MyCPU.hh"
#include "sim/insttracer.hh"

class MyCPU : public BaseCPU
{
  public:
    class CPUPort : public RequestPort
    {
      private:
        MemoryRequest *outstandingRequest;
        PacketPtr blockedPacket;

      public:
        CPUPort(const std::string &name, MyCPU *owner) :
            RequestPort(name, owner),
            outstandingRequest(nullptr),
            blockedPacket(nullptr)
        { }

        void sendPacket(MemoryRequest *request, PacketPtr pkt);

        bool isBlocked() { return outstandingRequest != nullptr; }

      protected:
        bool recvTimingResp(PacketPtr pkt) override;
        void recvReqRetry() override;
    };

    class IcachePort : public CPUPort
    {
      public:
        IcachePort(MyCPU *owner) :
            CPUPort(owner->name() + ".icahce_port", owner)
        { }
    };

    class DcachePort : public CPUPort
    {
      public:
        DcachePort(MyCPU *owner) :
            CPUPort(owner->name() + ".dcache_port", owner)
        { }
    };

  private:
    IcachePort iport;
    DcachePort dport;
    SimpleThread thread;
    StaticInstPtr currentMacroOp;
    Trace::InstRecord *traceData;

    void fetch(Addr offset = 0);
    void executeInstruction(StaticInstPtr inst);
    void finishExecute(StaticInstPtr inst, const Fault &fault);

  public:
    MyCPU(MyCPUParams *params);

    void init() override;
    void startup() override;
    void activateContext(ThreadID tid) override;
    void wakeup(ThreadID tid) override;

    RequestPort &getDataPort() override { return dport; }
    RequestPort &getInstPort() override { return iport; }
    Counter totalInsts() const override { return 0; }
    Counter totalOps() const override { return 0; }

    void finishFetchTranslate(MemoryRequest *request);
    void finishDataTranslate(MemoryRequest *request);
    void decodeInstrcution(PacketPtr pkt);
    void dataResponse(PacketPtr pkt, StaticInstPtr inst);
};

#endif // __CPU_MYCPU_CPU_HH__


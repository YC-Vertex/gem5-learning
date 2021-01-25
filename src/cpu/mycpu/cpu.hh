#ifndef __CPU_MYCPU_CPU_HH__
#define __CPU_MYCPU_CPU_HH__

class MyCPU : public BaseCPU
{
  public:
    class CPUPort : public MasterPort
    {
    };

    class IcachePort : public CPUPort
    {
    };

    class DcachePort : public CPUPort
    {
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

    RequestPort &getDataPort() override;
    RequestPort &getInstPort() override;
    Counter totalInsts() const override;
    Counter totalOps() const override;

    void finishFetchTranslate(MemoryRequest *request);
    void finishDataTranslate(MemoryRequest *request);
    void decodeInstrcution(PacketPtr pkt);
    void dataResponse(PacketPtr pkt, StaticInstPtr inst);
};

#endif // __CPU_MYCPU_CPU_HH__


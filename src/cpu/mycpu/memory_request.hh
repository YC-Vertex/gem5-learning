#ifndef __MYCPU_MEMORY_REQUEST_HH__
#define __MYCPU_MEMORY_REQUEST_HH__

#include "config/the_isa.hh"
#include "cpu/mycpu/cpu.hh"
#include "cpu/simple_thread.hh"

class MyCPU;

class MemoryRequest : public BaseTLB::Translation
{
  private:
    MyCPU &cpu;
    SimpleThread &thread;
    StaticInstPtr inst;

    uint8_t *data;
    uint64_t *res;

    bool isFetch;
    bool isRead;
    bool isSplit;

    RequestPtr req;
    PacketPtr pkt;

    Fault fault;

    RequestPtr sreqLow;
    RequestPtr sreqHigh;
    PacketPtr spktLow;
    PacketPtr spktHigh;

    int waitingFor;

    void sendFetch();
    void sendData();
    void sendDataSplit();

  public:
    // constructor for reads and writes
    MemoryRequest(MyCPU &cpu, SimpleThread &thread, StaticInstPtr inst,
        Addr addr, unsigned int size, Request::Flags flags,
        uint8_t *data = nullptr, uint64_t *res = nullptr);
    // constructor for instruction fetches
    MemoryRequest(MyCPU &cpu, SimpleThread &thread, Addr addr);
    // destructor
    ~MemoryRequest();

    void translate();
    void finish(const Fault &faultm RequestPtr req,
        ThreadContext *tc, BaseTLB::Mode mode) override;
    void send();
    void markDelayed() override { };

    bool recvResponse(PacketPtr pkt);

    void trace(Trace::InstRecord *trace);

    RequestPtr getReq() { return req; }
    PacketPtr getPkt() { return pkt; }
    StaticInstPtr getInst() { return inst; }
    bool getIsFetch() { return isFetch; }
    bool getIsRead() { return isRead; }
    const Fault &getFault() { return fault; }
};

#endif // __MYCPU_MEMORY_REQUEST_HH__


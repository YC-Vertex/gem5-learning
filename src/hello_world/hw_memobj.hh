#ifndef __HELLO_WORLD_HW_MEMOBJ_HH__
#define __HELLO_WORLD_HW_MEMOBJ_HH__

// import paramters from Python
#include "params/HwMemobj.hh"

// include base classes
#include "sim/sim_object.hh"
#include "mem/port.hh"

class HwMemobj : public SimObject
{
  private:

    // cpu-side port type
    class CPUSidePort : public ResponsePort
    {
      private:
        HwMemobj *owner;
        bool needRetry;
        PacketPtr blockedPacket;

      public:
        CPUSidePort(const std::string &name, HwMemobj *owner):
            ResponsePort(name, owner), owner(owner) {}
        AddrRangeList getAddrRanges() const override;
        void sendPacket(PacketPtr pkt);
        void trySendRetry();

      protected:
        Tick recvAtomic(PacketPtr pkt) override { panic("recvAtomic unimpl."); }
        void recvFunctional(PacketPtr pkt) override;
        bool recvTimingReq(PacketPtr pkt) override;
        void recvRespRetry() override;
    };

    // mem-side port type
    class MemSidePort : public RequestPort
    {
      private:
        HwMemobj *owner;
        PacketPtr blockedPacket;
      
      public:
        MemSidePort(const std::string &name, HwMemobj *owner) :
            RequestPort(name, owner), owner(owner) {}
        void sendPacket(PacketPtr pkt);

      protected:
        bool recvTimingResp(PacketPtr pkt) override;
        void recvReqRetry() override;
        void recvRangeChange() override;
    };

    CPUSidePort instPort;
    CPUSidePort dataPort;
    MemSidePort memPort;

    bool blocked;

    AddrRangeList getAddrRanges() const;
    void sendRangeChange();

    bool handleRequest(PacketPtr pkt);
    bool handleResponse(PacketPtr pkt);
    void handleFunctional(PacketPtr pkt);

  public:

    // constructor
    HwMemobj(HwMemobjParams *params);

    Port &getPort(const std::string &if_name,
        PortID idx=InvalidPortID) override;
};

#endif // __HELLO_WORLD_HW_MEMOBJ_HH__


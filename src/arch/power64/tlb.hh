#ifndef __ARCH_POWER64_TLB_HH__
#define __ARCH_POWER64_TLB_HH__

#include <map>

#include "arch/generic/tlb.hh"
#include "arch/power64/isa_traits.hh"
#include "arch/power64/pagetable.hh"
#include "arch/power64/utility.hh"
#include "base/statistics.hh"
#include "mem/request.hh"
#include "params/Power64TLB.hh"

class ThreadContext;

namespace Power64ISA {

// This is copied from the ARM ISA and has not been checked against the
// Power at all.
struct TlbEntry
{
    Addr _pageStart;

    TlbEntry()
    {
    }

    TlbEntry(Addr asn, Addr vaddr, Addr paddr,
             bool uncacheable, bool read_only)
        : _pageStart(paddr)
    {
        if (uncacheable || read_only)
            warn("Power TlbEntry does not support uncacheable"
                 " or read-only mappings\n");
    }

    void
    updateVaddr(Addr new_vaddr)
    {
        panic("unimplemented");
    }

    Addr
    pageStart()
    {
        return _pageStart;
    }

    void
    serialize(CheckpointOut &cp) const
    {
        SERIALIZE_SCALAR(_pageStart);
    }

    void
    unserialize(CheckpointIn &cp)
    {
        UNSERIALIZE_SCALAR(_pageStart);
    }
};

class TLB : public BaseTLB
{
  protected:
    typedef std::multimap<Addr, int> PageTable;
    PageTable lookupTable;      // Quick lookup into page table

    Power64ISA::PTE *table;       // the Page Table
    int size;                   // TLB Size
    int nlu;                    // not last used entry (for replacement)

    void
    nextnlu()
    {
        if (++nlu >= size) {
            nlu = 0;
        }
    }

    Power64ISA::PTE *lookup(Addr vpn, uint8_t asn) const;

  public:
    typedef Power64TLBParams Params;
    TLB(const Params *p);
    virtual ~TLB();

    void takeOverFrom(BaseTLB *otlb) override {}

    int probeEntry(Addr vpn,uint8_t) const;
    Power64ISA::PTE *getEntry(unsigned) const;

    int smallPages;

    int
    getsize() const
    {
        return size;
    }

    Power64ISA::PTE &index(bool advance = true);
    void insert(Addr vaddr, Power64ISA::PTE &pte);
    void insertAt(Power64ISA::PTE &pte, unsigned Index, int _smallPages);
    void flushAll() override;

    void
    demapPage(Addr vaddr, uint64_t asn) override
    {
        panic("demapPage unimplemented.\n");
    }

    // static helper functions... really
    static bool validVirtualAddress(Addr vaddr);
    static Fault checkCacheability(const RequestPtr &req);
    Fault translateInst(const RequestPtr &req, ThreadContext *tc);
    Fault translateData(const RequestPtr &req, ThreadContext *tc, bool write);
    Fault translateAtomic(
            const RequestPtr &req, ThreadContext *tc, Mode mode) override;
    void translateTiming(
            const RequestPtr &req, ThreadContext *tc,
            Translation *translation, Mode mode) override;
    Fault translateFunctional(
            const RequestPtr &req, ThreadContext *tc, Mode mode) override;
    Fault finalizePhysical(
            const RequestPtr &req,
            ThreadContext *tc, Mode mode) const override;

    // Checkpointing
    void serialize(CheckpointOut &cp) const override;
    void unserialize(CheckpointIn &cp) override;
};

} // namespace Power64ISA

#endif // __ARCH_POWER64_TLB_HH__

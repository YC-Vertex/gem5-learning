#include "arch/power64/tlb.hh"

#include <string>
#include <vector>

#include "arch/power64/faults.hh"
#include "arch/power64/pagetable.hh"
#include "arch/power64/utility.hh"
#include "base/inifile.hh"
#include "base/str.hh"
#include "base/trace.hh"
#include "cpu/thread_context.hh"
#include "debug/Power64.hh"
#include "debug/TLB.hh"
#include "mem/page_table.hh"
#include "params/Power64TLB.hh"
#include "sim/full_system.hh"
#include "sim/process.hh"

using namespace std;
using namespace Power64ISA;

///////////////////////////////////////////////////////////////////////
//
//  POWER TLB
//

#define MODE2MASK(X) (1 << (X))

TLB::TLB(const Params *p)
    : BaseTLB(p), size(p->size), nlu(0)
{
    table = new Power64ISA::PTE[size];
    memset(table, 0, sizeof(Power64ISA::PTE[size]));
    smallPages = 0;
}

TLB::~TLB()
{
    if (table)
        delete [] table;
}

// look up an entry in the TLB
Power64ISA::PTE *
TLB::lookup(Addr vpn, uint8_t asn) const
{
    // assume not found...
    Power64ISA::PTE *retval = NULL;
    PageTable::const_iterator i = lookupTable.find(vpn);
    if (i != lookupTable.end()) {
        while (i->first == vpn) {
            int index = i->second;
            Power64ISA::PTE *pte = &table[index];
            Addr Mask = pte->Mask;
            Addr InvMask = ~Mask;
            Addr VPN  = pte->VPN;
            if (((vpn & InvMask) == (VPN & InvMask))
               && (pte->G  || (asn == pte->asid))) {

                // We have a VPN + ASID Match
                retval = pte;
                break;
            }
            ++i;
        }
    }

    DPRINTF(TLB, "lookup %#x, asn %#x -> %s ppn %#x\n", vpn, (int)asn,
            retval ? "hit" : "miss", retval ? retval->PFN1 : 0);
    return retval;
}

Power64ISA::PTE*
TLB::getEntry(unsigned Index) const
{
    // Make sure that Index is valid
    assert(Index<size);
    return &table[Index];
}

int
TLB::probeEntry(Addr vpn,uint8_t asn) const
{
    // assume not found...
    int Ind = -1;
    PageTable::const_iterator i = lookupTable.find(vpn);
    if (i != lookupTable.end()) {
        while (i->first == vpn) {
            int index = i->second;
            Power64ISA::PTE *pte = &table[index];
            Addr Mask = pte->Mask;
            Addr InvMask = ~Mask;
            Addr VPN  = pte->VPN;
            if (((vpn & InvMask) == (VPN & InvMask))
                && (pte->G  || (asn == pte->asid))) {

                // We have a VPN + ASID Match
                Ind = index;
                break;
            }
            ++i;
        }
    }

    DPRINTF(Power64, "VPN: %x, asid: %d, Result of TLBP: %d\n", vpn, asn, Ind);
    return Ind;
}

inline Fault
TLB::checkCacheability(const RequestPtr &req)
{
    Addr VAddrUncacheable = 0xA0000000;
    if ((req->getVaddr() & VAddrUncacheable) == VAddrUncacheable) {

        // mark request as uncacheable
        req->setFlags(Request::UNCACHEABLE | Request::STRICT_ORDER);
    }
    return NoFault;
}

void
TLB::insertAt(Power64ISA::PTE &pte, unsigned Index, int _smallPages)
{
    smallPages=_smallPages;
    if (Index > size){
        warn("Attempted to write at index (%d) beyond TLB size (%d)",
             Index, size);
    } else {

        // Update TLB
        if (table[Index].V0 || table[Index].V1) {

            // Previous entry is valid
            PageTable::iterator i = lookupTable.find(table[Index].VPN);
            lookupTable.erase(i);
        }
        table[Index]=pte;

        // Update fast lookup table
        lookupTable.insert(make_pair(table[Index].VPN, Index));
    }
}

// insert a new TLB entry
void
TLB::insert(Addr addr, Power64ISA::PTE &pte)
{
    fatal("TLB Insert not yet implemented\n");
}

void
TLB::flushAll()
{
    DPRINTF(TLB, "flushAll\n");
    memset(table, 0, sizeof(Power64ISA::PTE[size]));
    lookupTable.clear();
    nlu = 0;
}

void
TLB::serialize(CheckpointOut &cp) const
{
    SERIALIZE_SCALAR(size);
    SERIALIZE_SCALAR(nlu);

    for (int i = 0; i < size; i++) {
        ScopedCheckpointSection sec(cp, csprintf("PTE%d", i));
        table[i].serialize(cp);
    }
}

void
TLB::unserialize(CheckpointIn &cp)
{
    UNSERIALIZE_SCALAR(size);
    UNSERIALIZE_SCALAR(nlu);

    for (int i = 0; i < size; i++) {
        ScopedCheckpointSection sec(cp, csprintf("PTE%d", i));
        if (table[i].V0 || table[i].V1) {
            lookupTable.insert(make_pair(table[i].VPN, i));
        }
    }
}

Fault
TLB::translateInst(const RequestPtr &req, ThreadContext *tc)
{
    // Instruction accesses must be word-aligned
    if (req->getVaddr() & 0x3) {
        DPRINTF(TLB, "Alignment Fault on %#x, size = %d\n", req->getVaddr(),
                req->getSize());
        return std::make_shared<AlignmentFault>();
    }

    return tc->getProcessPtr()->pTable->translate(req);
}

Fault
TLB::translateData(const RequestPtr &req, ThreadContext *tc, bool write)
{
    return tc->getProcessPtr()->pTable->translate(req);
}

Fault
TLB::translateAtomic(const RequestPtr &req, ThreadContext *tc, Mode mode)
{
    panic_if(FullSystem,
            "translateAtomic not yet implemented for full system.");

    if (mode == Execute)
        return translateInst(req, tc);
    else
        return translateData(req, tc, mode == Write);
}

Fault
TLB::translateFunctional(const RequestPtr &req, ThreadContext *tc, Mode mode)
{
    panic_if(FullSystem,
            "translateFunctional not implemented for full system.");
    return tc->getProcessPtr()->pTable->translate(req);
}

void
TLB::translateTiming(const RequestPtr &req, ThreadContext *tc,
                     Translation *translation, Mode mode)
{
    assert(translation);
    translation->finish(translateAtomic(req, tc, mode), req, tc, mode);
}

Fault
TLB::finalizePhysical(const RequestPtr &req,
                      ThreadContext *tc, Mode mode) const
{
    return NoFault;
}

Power64ISA::PTE &
TLB::index(bool advance)
{
    Power64ISA::PTE *pte = &table[nlu];

    if (advance)
        nextnlu();

    return *pte;
}

Power64ISA::TLB *
Power64TLBParams::create()
{
    return new Power64ISA::TLB(this);
}

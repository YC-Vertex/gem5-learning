/*
 * $NetBSD: kgdb_stub.c,v 1.8 2001/07/07 22:58:00 wdk Exp $
 *
 * Taken from NetBSD
 *
 * "Stub" to allow remote cpu to debug over a serial line using gdb.
 */


#include "arch/power64/remote_gdb.hh"

#include <sys/signal.h>
#include <unistd.h>

#include <string>

#include "blobs/gdb_xml_power.hh"
#include "cpu/thread_state.hh"
#include "debug/GDBAcc.hh"
#include "debug/GDBMisc.hh"
#include "mem/page_table.hh"
#include "sim/byteswap.hh"

using namespace std;
using namespace Power64ISA;

RemoteGDB::RemoteGDB(System *_system, ThreadContext *tc, int _port)
    : BaseRemoteGDB(_system, tc, _port), regCache(this)
{
}

/*
 * Determine if the mapping at va..(va+len) is valid.
 */
bool
RemoteGDB::acc(Addr va, size_t len)
{
    // Check to make sure the first byte is mapped into the processes address
    // space.  At the time of this writing, the acc() check is used when
    // processing the MemR/MemW packets before actually asking the translating
    // port proxy to read/writeBlob.  I (bgs) am not convinced the first byte
    // check is enough.
    panic_if(FullSystem, "acc not implemented for POWER FS!");
    return context()->getProcessPtr()->pTable->lookup(va) != nullptr;
}

void
RemoteGDB::PowerGdbRegCache::getRegs(ThreadContext *context)
{
    DPRINTF(GDBAcc, "getRegs in remotegdb \n");

    // Default order on 32-bit PowerPC:
    // R0-R31 (32-bit each), F0-F31 (64-bit IEEE754 double),
    // PC, MSR, CR, LR, CTR, XER (32-bit each)

    for (int i = 0; i < NumIntArchRegs; i++)
        r.gpr[i] = htobe((uint32_t)context->readIntReg(i));

    for (int i = 0; i < NumFloatArchRegs; i++)
        r.fpr[i] = context->readFloatReg(i);

    r.pc = htobe((uint32_t)context->pcState().pc());
    r.msr = 0; // Is MSR modeled?
    r.cr = htobe((uint32_t)context->readIntReg(INTREG_CR));
    r.lr = htobe((uint32_t)context->readIntReg(INTREG_LR));
    r.ctr = htobe((uint32_t)context->readIntReg(INTREG_CTR));
    r.xer = htobe((uint32_t)context->readIntReg(INTREG_XER));
}

void
RemoteGDB::PowerGdbRegCache::setRegs(ThreadContext *context) const
{
    DPRINTF(GDBAcc, "setRegs in remotegdb \n");

    for (int i = 0; i < NumIntArchRegs; i++)
        context->setIntReg(i, betoh(r.gpr[i]));

    for (int i = 0; i < NumFloatArchRegs; i++)
        context->setFloatReg(i, r.fpr[i]);

    context->pcState(betoh(r.pc));
    // Is MSR modeled?
    context->setIntReg(INTREG_CR, betoh(r.cr));
    context->setIntReg(INTREG_LR, betoh(r.lr));
    context->setIntReg(INTREG_CTR, betoh(r.ctr));
    context->setIntReg(INTREG_XER, betoh(r.xer));
}

BaseGdbRegCache*
RemoteGDB::gdbRegs()
{
    return &regCache;
}

bool
RemoteGDB::getXferFeaturesRead(const std::string &annex, std::string &output)
{
#define GDB_XML(x, s) \
        { x, std::string(reinterpret_cast<const char *>(Blobs::s), \
        Blobs::s ## _len) }
    static const std::map<std::string, std::string> annexMap {
        GDB_XML("target.xml", gdb_xml_power),
    };
#undef GDB_XML
    auto it = annexMap.find(annex);
    if (it == annexMap.end())
        return false;
    output = it->second;
    return true;
}

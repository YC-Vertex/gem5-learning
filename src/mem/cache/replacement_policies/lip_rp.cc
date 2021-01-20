#include "mem/cache/replacement_policies/lip_rp.hh"

#include <memory>

#include "params/LIPRPs.hh"
#include "sim/core.hh"

LIPRP::LIPRP(const Params *p) :
    BaseReplacementPolicy(p)
{
}

LIPRP::pLIPReplData
LIPRP::ptrCast(const pReplData ptr) const
{
    return std::static_pointer_cast<LIPReplData>(ptr);
}

void
LIPRP::reset(const pReplData &repl_data) const
{
    auto p = ptrCast(repl_data);
    p->lastTouchTick = Tick(0);
}

void
LIPRP::touch(const pReplData &repl_data) const
{
    auto p = ptrCast(repl_data);
    p->lastTouchTick = Tick(0);
}

void 
LIPRP::invalidate(const pReplData &repl_data) const
{
    auto p = ptrCast(repl_data);
    p->lastTouchTick = curTick();
}

ReplaceableEntry*
LIPRP::getVictim(const ReplacementCandidates& candidates) const
{
    assert(candidates.size() > 0);

    ReplaceableEntry* victim = candidates[0];
    for (const auto& candidate : candidates) {
        if (ptrCast(candidate->replacementData)->lastTouchTick <
                ptrCast(victim->replacementData)->lastTouchTick) {
            victim = candidate;
        }
    }

    return victim;
}

LIPRP::pReplData
LIPRP::instantiateEntry()
{
    return pReplData(new LIPReplData());
}

LIPRP*
LIPRPsParams::create()
{
    return new LIPRP(this);
}


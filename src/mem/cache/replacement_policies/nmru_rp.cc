#include "mem/cache/replacement_policies/nmru_rp.hh"

#include <memory>

#include "params/NMRURP.hh"
#include "sim/core.hh"

NMRURP::NMRURP(const Params *p) :
    BaseReplacementPolicy(p)
{
}

NMRURP::pNMRUReplData
NMRURP::ptrCast(const pReplData ptr) const
{
    return std::static_pointer_cast<NMRUReplData>(ptr);
}

void
NMRURP::reset(const pReplData &repl_data) const
{
    auto p = ptrCast(repl_data);
    p->lastTouchTick = Tick(0);
}

void
NMRURP::touch(const pReplData &repl_data) const
{
    auto p = ptrCast(repl_data);
    p->lastTouchTick = curTick();
}

void 
NMRURP::invalidate(const pReplData &repl_data) const
{
    auto p = ptrCast(repl_data);
    p->lastTouchTick = curTick();
}

ReplaceableEntry*
NMRURP::getVictim(const ReplacementCandidates& candidates) const
{
    assert(candidates.size() > 0);

    ReplaceableEntry *victim = candidates[0];
    for (const auto& candidate : candidates) {
        if (ptrCast(candidate->replacementData)->lastTouchTick
            <= ptrCast(victim->replacementData)->lastTouchTick) {
            victim = candidate;
        }
    }

    return victim;
}

NMRURP::pReplData
NMRURP::instantiateEntry()
{
    return pReplData(new NMRUReplData());
}

NMRURP*
NMRURPParams::create()
{
    return new NMRURP(this);
}


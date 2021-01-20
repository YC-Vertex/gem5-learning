#include "mem/cache/replacement_policies/nmru_rp.hh"

#include <memory>

#include "base/random.hh"
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

    unsigned int mru_index = 0;
    Tick mru_tick = 0;
    for (unsigned int i = 0; i < candidates.size(); ++i) {
        const auto &candidate = candidates[i];
        Tick cand_tick = ptrCast(candidate->replacementData)->lastTouchTick;
        if (cand_tick > mru_tick) {
            mru_tick = cand_tick;
            mru_index = i;
        }
    }

    unsigned int victim_index = 0;
    do {
        random_mt.random<unsigned>(0, candidates.size()-1);
    } while (victim_index == mru_index);

    return candidates[victim_index];
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


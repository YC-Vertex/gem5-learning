#ifndef __MEM_CACHE_REPLACEMENT_POLICIES_NMRU_RP_HH__
#define __MEM_CACHE_REPLACEMENT_POLICIES_NMRU_RP_HH__

#include "mem/cache/replacement_policies/base.hh"

struct NMRURPParams;

class NMRURP : public BaseReplacementPolicy
{
  protected:

    struct NMRUReplData : ReplacementData
    {
        Tick lastTouchTick;
        NMRUReplData() : lastTouchTick(0) {}
    };

  public:

    typedef NMRURPParams Params;
    using pReplData = std::shared_ptr<ReplacementData>;
    using pNMRUReplData = std::shared_ptr<NMRUReplData>;

    NMRURP(const Params *p);
    ~NMRURP() {}
    
    void reset(const pReplData &repl_data) const override;
    void touch(const pReplData &repl_data) const override;
    void invalidate(const pReplData &repl_data) const override;
    ReplaceableEntry* getVictim(
        const ReplacementCandidates& candidates) const override;

    pReplData instantiateEntry() override;

  private:

    pNMRUReplData ptrCast(pReplData ptr) const;
};

#endif // __MEM_CACHE_REPLACEMENT_POLICIES_NMRU_RP_HH__


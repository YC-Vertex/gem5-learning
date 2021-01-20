#ifndef __MEM_CACHE_REPLACEMENT_POLICIES_LIP_RP_HH__
#define __MEM_CACHE_REPLACEMENT_POLICIES_LIP_RP_HH__

#include "mem/cache/replacement_policies/base.hh"

struct LIPRPsParams;

class LIPRP : public BaseReplacementPolicy
{
  protected:

    struct LIPReplData : ReplacementData
    {
        Tick lastTouchTick;
        LIPReplData() : lastTouchTick(0) {}
    };

  public:

    typedef LIPRPsParams Params;
    using pReplData = std::shared_ptr<ReplacementData>;
    using pLIPReplData = std::shared_ptr<LIPReplData>;

    LIPRP(const Params *p);
    ~LIPRP() {}
    
    void reset(const pReplData &repl_data) const override;
    void touch(const pReplData &repl_data) const override;
    void invalidate(const pReplData &repl_data) const override;
    ReplaceableEntry* getVictim(
        const ReplacementCandidates& candidates) const override;

    pReplData instantiateEntry() override;

  private:

    pLIPReplData ptrCast(pReplData ptr) const;
};

#endif // __MEM_CACHE_REPLACEMENT_POLICIES_LIP_RP_HH__


#ifndef __ARCH_POWER64_DECODER_HH__
#define __ARCH_POWER64_DECODER_HH__

#include "arch/generic/decode_cache.hh"
#include "arch/generic/decoder.hh"
#include "arch/types.hh"
#include "cpu/static_inst.hh"

namespace Power64ISA
{

class ISA;
class Decoder : public InstDecoder
{
  protected:
    // The extended machine instruction being generated
    ExtMachInst emi;
    bool instDone;

  public:
    Decoder(ISA* isa = nullptr) : instDone(false)
    {
    }

    void
    process()
    {
    }

    void
    reset()
    {
        instDone = false;
    }

    // Use this to give data to the predecoder. This should be used
    // when there is control flow.
    void
    moreBytes(const PCState &pc, Addr fetchPC, MachInst inst)
    {
        emi = betoh(inst);
        instDone = true;
    }

    // Use this to give data to the predecoder. This should be used
    // when instructions are executed in order.
    void
    moreBytes(MachInst machInst)
    {
        moreBytes(0, 0, machInst);
    }

    bool
    needMoreBytes()
    {
        return true;
    }

    bool
    instReady()
    {
        return instDone;
    }

    void takeOverFrom(Decoder *old) {}

  protected:
    /// A cache of decoded instruction objects.
    static GenericISA::BasicDecodeCache defaultCache;

  public:
    StaticInstPtr decodeInst(ExtMachInst mach_inst);

    /// Decode a machine instruction.
    /// @param mach_inst The binary instruction to decode.
    /// @retval A pointer to the corresponding StaticInst object.
    StaticInstPtr
    decode(ExtMachInst mach_inst, Addr addr)
    {
        return defaultCache.decode(this, mach_inst, addr);
    }

    StaticInstPtr
    decode(Power64ISA::PCState &nextPC)
    {
        if (!instDone)
            return NULL;
        instDone = false;
        return decode(emi, nextPC.instAddr());
    }
};

} // namespace Power64ISA

#endif // __ARCH_POWER64_DECODER_HH__

#ifndef __ARCH_POWER64_REGISTERS_HH__
#define __ARCH_POWER64_REGISTERS_HH__

#include "arch/generic/vec_pred_reg.hh"
#include "arch/generic/vec_reg.hh"
#include "arch/power64/generated/max_inst_regs.hh"
#include "arch/power64/miscregs.hh"
#include "base/types.hh"

namespace Power64ISA {

using Power64ISAInst::MaxInstSrcRegs;
using Power64ISAInst::MaxInstDestRegs;

// Power writes a misc register outside of the isa parser, so it can't
// be detected by it. Manually add it here.
const int MaxMiscDestRegs = Power64ISAInst::MaxMiscDestRegs + 1;

// Not applicable to Power
using VecElem = ::DummyVecElem;
using VecReg = ::DummyVecReg;
using ConstVecReg = ::DummyConstVecReg;
using VecRegContainer = ::DummyVecRegContainer;
constexpr unsigned NumVecElemPerVecReg = ::DummyNumVecElemPerVecReg;
constexpr size_t VecRegSizeBytes = ::DummyVecRegSizeBytes;

// Not applicable to Power
using VecPredReg = ::DummyVecPredReg;
using ConstVecPredReg = ::DummyConstVecPredReg;
using VecPredRegContainer = ::DummyVecPredRegContainer;
constexpr size_t VecPredRegSizeBits = ::DummyVecPredRegSizeBits;
constexpr bool VecPredRegHasPackedRepr = ::DummyVecPredRegHasPackedRepr;

// Constants Related to the number of registers
const int NumIntArchRegs = 32;

// CR, XER, LR, CTR, FPSCR, RSV, RSV-LEN, RSV-ADDR
// and zero register, which doesn't actually exist but needs a number
const int NumIntSpecialRegs = 9;
const int NumFloatArchRegs = 32;
const int NumFloatSpecialRegs = 0;
const int NumInternalProcRegs = 0;

const int NumIntRegs = NumIntArchRegs + NumIntSpecialRegs;
const int NumFloatRegs = NumFloatArchRegs + NumFloatSpecialRegs;
const int NumVecRegs = 1;  // Not applicable to Power
                           // (1 to prevent warnings)
const int NumVecPredRegs = 1;  // Not applicable to Power
                               // (1 to prevent warnings)
const int NumCCRegs = 0;
const int NumMiscRegs = NUM_MISCREGS;

// Semantically meaningful register indices
const int ReturnValueReg = 3;
const int ArgumentReg0 = 3;
const int ArgumentReg1 = 4;
const int ArgumentReg2 = 5;
const int ArgumentReg3 = 6;
const int ArgumentReg4 = 7;
const int FramePointerReg = 31;
const int StackPointerReg = 1;

// There isn't one in Power, but we need to define one somewhere
const int ZeroReg = NumIntRegs - 1;

const int SyscallNumReg = 0;
const int SyscallPseudoReturnReg = 3;
const int SyscallSuccessReg = 3;

enum MiscIntRegNums {
    INTREG_CR = NumIntArchRegs,
    INTREG_XER,
    INTREG_LR,
    INTREG_CTR,
    INTREG_FPSCR,
    INTREG_RSV,
    INTREG_RSV_LEN,
    INTREG_RSV_ADDR
};

} // namespace Power64ISA

#endif // __ARCH_POWER64_REGISTERS_HH__

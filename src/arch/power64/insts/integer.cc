#include "arch/power64/insts/integer.hh"

using namespace std;
using namespace Power64ISA;

string
IntOp::generateDisassembly(Addr pc, const Loader::SymbolTable *symtab) const
{
    stringstream ss;
    bool printDest = true;
    bool printSrcs = true;
    bool printSecondSrc = true;

    // Generate the correct mnemonic
    string myMnemonic(mnemonic);

    // Special cases
    if (!myMnemonic.compare("or") && _srcRegIdx[0] == _srcRegIdx[1]) {
        myMnemonic = "mr";
        printSecondSrc = false;
    } else if (!myMnemonic.compare("mtlr") || !myMnemonic.compare("cmpi")) {
        printDest = false;
    } else if (!myMnemonic.compare("mflr")) {
        printSrcs = false;
    }

    // Additional characters depending on isa bits being set
    if (oeSet) myMnemonic = myMnemonic + "o";
    if (rcSet) myMnemonic = myMnemonic + ".";
    ccprintf(ss, "%-10s ", myMnemonic);

    // Print the first destination only
    if (_numDestRegs > 0 && printDest) {
        printReg(ss, _destRegIdx[0]);
    }

    // Print the (possibly) two source registers
    if (_numSrcRegs > 0 && printSrcs) {
        if (_numDestRegs > 0 && printDest) {
            ss << ", ";
        }
        printReg(ss, _srcRegIdx[0]);
        if (_numSrcRegs > 1 && printSecondSrc) {
          ss << ", ";
          printReg(ss, _srcRegIdx[1]);
        }
    }

    return ss.str();
}


string
IntImmOp::generateDisassembly(Addr pc, const Loader::SymbolTable *symtab) const
{
    stringstream ss;

    // Generate the correct mnemonic
    string myMnemonic(mnemonic);

    // Special cases
    if (!myMnemonic.compare("addi") && _numSrcRegs == 0) {
        myMnemonic = "li";
    } else if (!myMnemonic.compare("addis") && _numSrcRegs == 0) {
        myMnemonic = "lis";
    }
    ccprintf(ss, "%-10s ", myMnemonic);

    // Print the first destination only
    if (_numDestRegs > 0) {
        printReg(ss, _destRegIdx[0]);
    }

    // Print the source register
    if (_numSrcRegs > 0) {
        if (_numDestRegs > 0) {
            ss << ", ";
        }
        printReg(ss, _srcRegIdx[0]);
    }

    // Print the immediate value last
    ss << ", " << (int32_t)imm;

    return ss.str();
}


string
IntShiftOp::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    stringstream ss;

    ccprintf(ss, "%-10s ", mnemonic);

    // Print the first destination only
    if (_numDestRegs > 0) {
        printReg(ss, _destRegIdx[0]);
    }

    // Print the first source register
    if (_numSrcRegs > 0) {
        if (_numDestRegs > 0) {
            ss << ", ";
        }
        printReg(ss, _srcRegIdx[0]);
    }

    // Print the shift
    ss << ", " << sh;

    return ss.str();
}


string
IntRotateOp::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    stringstream ss;

    ccprintf(ss, "%-10s ", mnemonic);

    // Print the first destination only
    if (_numDestRegs > 0) {
        printReg(ss, _destRegIdx[0]);
    }

    // Print the first source register
    if (_numSrcRegs > 0) {
        if (_numDestRegs > 0) {
            ss << ", ";
        }
        printReg(ss, _srcRegIdx[0]);
    }

    // Print the shift, mask begin and mask end
    ss << ", " << sh << ", " << mb << ", " << me;

    return ss.str();
}

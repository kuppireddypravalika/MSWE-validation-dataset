#ifndef OSR_SIM_HPP
#define OSR_SIM_HPP
#include <cstdint>
#include <vector>

constexpr int TR_induceOSRAtCurrentPC = 1;

enum OSRMode { involuntaryOSR, voluntaryOSR };

struct SymbolReference {
    int ref;
    int getReferenceNumber() const { return ref; }
};

struct OpCode {
    SymbolReference* sym;
    bool hasSymbolReference() const { return sym != nullptr; }
};

struct Node {
    SymbolReference* sym;
    OpCode getOpCode() { return OpCode{sym}; }
    SymbolReference* getSymbolReference() { return sym; }
};

struct ByteCodeInfo {
    int callerIndex;
    int byteCodeIndex;
};

struct Instruction {
    Node* node;
    int pc;
    ByteCodeInfo bcInfo;
    int getBinaryEncoding() const { return pc; }
    struct CodeGen { int getCodeStart() const { return 0; } } cg_;
    CodeGen* cg() { return &cg_; }
    Node* getNode() const { return node; }
};

struct Compilation {
    OSRMode mode;
    Compilation(OSRMode m) : mode(m) {}
    OSRMode getOSRMode() const { return mode; }
};

#endif

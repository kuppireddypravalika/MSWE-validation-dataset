#include <vector>
#include <cstdint>

struct DSP {
    using TWord = int;
    using InstructionFunc = void (DSP::*)(TWord);

    static InstructionFunc jumpTable[2];

    int pc = 0;
    volatile int acc = 0;
    volatile long long instructions = 0;
    bool trace = true;

    DSP() {}

    __attribute__((noinline)) void op_add(TWord v) { acc += v; }
    __attribute__((noinline)) void op_sub(TWord v) { acc -= v; }

    __attribute__((noinline)) void execOp(TWord op) {
        (this->*jumpTable[op])((op == 0) ? 1 : -1);
        ++instructions;
        if (trace) { volatile int t = acc; (void)t; }
    }

    bool rep_exec(int loopCount) {
        for (int i = 0; i < loopCount; ++i) {
            execOp(0);
        }
        return true;
    }
};

DSP::InstructionFunc DSP::jumpTable[2] = {&DSP::op_add, &DSP::op_sub};

int run(int repeat, int loops) {
    DSP d;
    for (int i = 0; i < repeat; ++i) {
        d.rep_exec(loops);
    }
    return d.acc;
}

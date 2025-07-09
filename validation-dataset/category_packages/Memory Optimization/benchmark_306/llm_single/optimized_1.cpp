#include <vector>
#include <cstdint>

struct DSP {
    using TWord = int;
    using InstructionFunc = void (DSP::*)(TWord);

    static InstructionFunc jumpTable[2];

    std::vector<int> opcodeCache;
    volatile int acc = 0;
    volatile long long instructions = 0;
    bool trace = true;

    DSP() : opcodeCache{0} {}

    inline void op_add(TWord v) { acc += v; }
    inline void op_sub(TWord v) { acc -= v; }

    inline void execOp(TWord op) {
        InstructionFunc func = jumpTable[opcodeCache[0]];
        (this->*func)(op);
        instructions++;
        if (trace) { volatile int t = acc; (void)t; }
    }

    bool rep_exec(int loopCount) {
        if (loopCount <= 0) return true;
        execOp(1);
        for (--loopCount; loopCount > 0; --loopCount) {
            execOp(1);
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

#include <array>
#include <cstdint>

static volatile int global_sink = 0;
struct CPUState {
    int sp;
    int framePtr;
    int acc;
    std::array<int, 1024> scratch;
};

static inline void BuildMI_t2MOVr(CPUState& st, int reg) {
    int tempAcc = st.acc;
    int* scratch = st.scratch.data();
    for (int i = 0; i < 1000; ++i) {
        tempAcc = tempAcc * 1664525 + 1013904223;
        scratch[(tempAcc >> 10) & 1023] ^= reg + tempAcc;
        global_sink += tempAcc;
    }
    st.sp = reg;
}

static inline void BuildMI_MOVr(CPUState& st, int reg) {
    int tempAcc = st.acc;
    int* scratch = st.scratch.data();
    for (int i = 0; i < 1000; ++i) {
        tempAcc = tempAcc * 1103515245 + 12345;
        scratch[(tempAcc >> 12) & 1023] += reg + tempAcc;
        global_sink += tempAcc;
    }
    st.sp = reg;
}

// Replicates the relevant section of ARMBaseRegisterInfo::emitEpilogue
int emit_epilogue(bool isARM, int numBytes, int repeat) {
    CPUState st{};
    st.framePtr = 42;
    st.sp = 0;
    st.acc = 1;

    for (int r = 0; r < repeat; ++r) {
        if (numBytes) {
            (isARM ? BuildMI_MOVr : BuildMI_t2MOVr)(st, st.framePtr);
        } else {
            (isARM ? BuildMI_MOVr : BuildMI_t2MOVr)(st, st.framePtr);
        }
    }

    return st.sp;
}

// Explicit template instantiation statements

#include <array>
#include <cstdint>

static volatile int global_sink = 0;
struct CPUState {
    int sp;
    int framePtr;
    int acc;
    std::array<int,1024> scratch;
};


static inline void BuildMI_t2MOVr(CPUState& st, int reg) {
    // Simulate the heavier t2MOVr instruction
    for (int i = 0; i < 1000; ++i) {
        st.acc = st.acc * 1664525 + 1013904223;
        st.scratch[(st.acc >> 10) & 1023] ^= reg + st.acc;
        global_sink += st.acc;
    }
    st.sp = reg;
}

static inline void BuildMI_MOVr(CPUState& st, int reg) {
    for (int i = 0; i < 1000; ++i) {
        st.acc = st.acc * 1103515245 + 12345;
        st.scratch[(st.acc >> 12) & 1023] += reg + st.acc;
        global_sink += st.acc;
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
            if (isARM)
                BuildMI_MOVr(st, st.framePtr);
            else
                BuildMI_t2MOVr(st, st.framePtr);
        } else {
            unsigned MOVrOpc = isARM ? 1 : 2; // stand-ins for opcodes
            if (MOVrOpc == 1)
                BuildMI_MOVr(st, st.framePtr);
            else
                BuildMI_t2MOVr(st, st.framePtr);
        }
    }

    return st.sp;
}

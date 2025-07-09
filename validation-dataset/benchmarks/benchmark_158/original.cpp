#include <vector>
#include <cstddef>

struct Use { bool isDebug; };

struct MachineBasicBlock {
    std::vector<unsigned> ins;
    void addLiveIn(unsigned reg) { ins.push_back(reg); }
};

struct MachineRegisterInfo {
    struct LiveInPair { unsigned first; unsigned second; };
    std::vector<LiveInPair> LiveIns;
    std::vector<std::vector<Use>> Uses;
    unsigned workload;
};

extern volatile unsigned long long sink;

static inline bool use_empty(const MachineRegisterInfo& mri, unsigned v) {
    return mri.Uses[v].empty();
}

static inline bool use_nodbg_empty(const MachineRegisterInfo& mri, unsigned v) {
    for (const auto& u : mri.Uses[v]) if (!u.isDebug) return false;
    return true;
}

static inline void heavy_copy(unsigned src, unsigned dst, unsigned iters) {
    for (unsigned i = 0; i < iters; ++i) {
        sink += src + dst + i;
    }
}

unsigned run_mri(MachineRegisterInfo& mri) {
    MachineBasicBlock mbb;
    for (unsigned i = 0, e = mri.LiveIns.size(); i != e; ++i) {
        if (mri.LiveIns[i].second) {
            if (use_empty(mri, mri.LiveIns[i].second)) {
                // drop
            } else {
                heavy_copy(mri.LiveIns[i].first, mri.LiveIns[i].second, mri.workload);
                mbb.addLiveIn(mri.LiveIns[i].first);
            }
        } else {
            mbb.addLiveIn(mri.LiveIns[i].first);
        }
    }
    return 0; // results are irrelevant
}

unsigned run_original(MachineRegisterInfo& mri) {
    return run_mri(mri);
}

unsigned run(MachineRegisterInfo& mri) {
    return run_original(mri);
}

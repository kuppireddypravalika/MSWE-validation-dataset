#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

struct Use { bool isDebug; };
struct MachineBasicBlock { std::vector<unsigned> ins; void addLiveIn(unsigned r){ ins.push_back(r); } };
struct MachineRegisterInfo {
    struct LiveInPair { unsigned first; unsigned second; };
    std::vector<LiveInPair> LiveIns;
    std::vector<std::vector<Use>> Uses;
    unsigned workload;
};

volatile unsigned long long sink = 0;

unsigned run_mri(MachineRegisterInfo& mri);

MachineRegisterInfo createMRI(size_t liveins, double debug_ratio, unsigned workload) {
    MachineRegisterInfo mri;
    mri.LiveIns.resize(liveins);
    mri.Uses.resize(liveins + 1);
    mri.workload = workload;
    std::mt19937 rng(12345);
    std::uniform_real_distribution<> dist(0.0,1.0);
    for (size_t i = 0; i < liveins; ++i) {
        mri.LiveIns[i] = {static_cast<unsigned>(i+1), static_cast<unsigned>(i+1)};
        bool dbg_only = dist(rng) < debug_ratio;
        if (dbg_only)
            mri.Uses[i+1].push_back({true});
        else
            mri.Uses[i+1].push_back({false});
    }
    return mri;
}

int main(int argc, char** argv) {
    std::string mode = "perf";
    size_t liveins = 1000;
    unsigned workload = 25000;
    double debug_ratio = 0.8;
    int repeat = 1330;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--liveins=",0)==0) liveins = std::stoul(arg.substr(10));
        else if (arg.rfind("--workload=",0)==0) workload = std::stoul(arg.substr(11));
        else if (arg.rfind("--debugratio=",0)==0) debug_ratio = std::stod(arg.substr(13));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        MachineRegisterInfo mri = createMRI(liveins, debug_ratio, workload);

        run_mri(mri);
        std::cout << 0 << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r=0;r<repeat;++r) {
            MachineRegisterInfo mri = createMRI(liveins, debug_ratio, workload);
            run_mri(mri);
        }
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }
    return 0;
}

#include <string>
#include <vector>
#include <functional>
#include <array>
#include <string_view>

struct Driver { std::string Dir; };

class ToolChain {
public:
    ToolChain(const Driver& D) : D(D) {}
    const Driver& getDriver() const { return D; }
    std::vector<std::string>& getProgramPaths() { return ProgramPaths; }
private:
    const Driver& D;
    std::vector<std::string> ProgramPaths;
};

class CudaInstallation {
public:
    CudaInstallation(std::string Bin, bool Valid) : BinPath(std::move(Bin)), IsValid(Valid) {}
    bool isValid() const { return IsValid; }
    const std::string& getBinPath() const { return BinPath; }
private:
    std::string BinPath;
    bool IsValid;
};

class CudaToolChainOriginal : public ToolChain {
public:
    CudaToolChainOriginal(const Driver& D, const ToolChain& HostTC,
                          std::string Bin, bool Valid)
        : ToolChain(D), HostTC(HostTC), Install(std::move(Bin), Valid) {
        if (Install.isValid())
            getProgramPaths().emplace_back(Install.getBinPath());
    }
private:
    const ToolChain& HostTC;
    CudaInstallation Install;
};

static const int FILES_PER_DIR = 1000;
static const int LOOP_REPEAT   = 150;

static std::string search_bundler(const std::vector<std::string>& paths,
                                  const std::string& driver_dir) {
    for (const std::string& dir : paths) {
        const bool is_driver = dir == driver_dir;
        for (int r = 0; r < LOOP_REPEAT; ++r) {
            for (int i = 0; i < FILES_PER_DIR; ++i) {
                const std::string f = dir + "/file" + std::to_string(i);
                volatile size_t h = std::hash<std::string>{}(f);
                (void)h;
                if (is_driver && i == FILES_PER_DIR - 1 && r == LOOP_REPEAT - 1)
                    return dir + "/clang-offload-bundler";
            }
        }
    }
    return driver_dir + "/clang-offload-bundler";
}

std::string locate_bundler() {
    Driver D{ "driver" };
    ToolChain host(D);
    CudaToolChainOriginal tc(D, host, "cuda_bin", true);
    auto& programPaths = tc.getProgramPaths();
    programPaths.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        programPaths.emplace_back("dir" + std::to_string(i));
    return search_bundler(programPaths, D.Dir);
}

// Explicit template instantiation

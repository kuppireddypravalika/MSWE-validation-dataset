#include <string>
#include <vector>
#include <functional>

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
    CudaInstallation(const std::string& Bin, bool Valid) : BinPath(Bin), IsValid(Valid) {}
    bool isValid() const { return IsValid; }
    const std::string& getBinPath() const { return BinPath; }
private:
    std::string BinPath;
    bool IsValid;
};

class CudaToolChainOriginal : public ToolChain {
public:
    CudaToolChainOriginal(const Driver& D, const ToolChain& HostTC,
                          const std::string& Bin, bool Valid)
        : ToolChain(D), HostTC(HostTC), Install(Bin, Valid) {
        if (Install.isValid())
            getProgramPaths().push_back(Install.getBinPath());
    }
private:
    const ToolChain& HostTC;
    CudaInstallation Install;
};

static const int FILES_PER_DIR = 1000;
static const int LOOP_REPEAT   = 150;

static std::string search_bundler(const std::vector<std::string>& paths,
                                  const std::string& driver_dir) {
    for (const auto& dir : paths) {
        bool is_driver = dir == driver_dir;
        if (is_driver) {
            // Only calculate the hash if we must return the bundler
            for (int r = 0; r < LOOP_REPEAT; ++r) {
                std::string f = dir + "/file" + std::to_string(FILES_PER_DIR - 1);
                volatile size_t h = std::hash<std::string>{}(f);
                (void)h;
                return dir + "/clang-offload-bundler";
            }
        }
        for (int r = 0; r < LOOP_REPEAT; ++r) {
            for (int i = 0; i < FILES_PER_DIR - 1; ++i) {
                std::string f = dir + "/file" + std::to_string(i);
                volatile size_t h = std::hash<std::string>{}(f);
                (void)h;
            }
        }
    }
    return driver_dir + "/clang-offload-bundler";
}

std::string locate_bundler() {
    Driver D{"driver"};
    ToolChain host(D);
    CudaToolChainOriginal tc(D, host, "cuda_bin", true);
    auto& program_paths = tc.getProgramPaths();
    program_paths.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        program_paths.emplace_back("dir" + std::to_string(i));
    return search_bundler(program_paths, D.Dir);
}

// Explicit template instantiation statements

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>

namespace demo {

using IntFunc = uint64_t(*)(int);

uint64_t unoptimized_body(int n); // from original.cpp
uint64_t optimized_body(int n) {
    return static_cast<uint64_t>(n) * n * (n - 1);
}

struct Module { IntFunc func; };
struct CodeGenAction {
    std::unique_ptr<Module> takeModule() {
        return std::make_unique<Module>(Module{unoptimized_body});
    }
};
struct SectionMemoryManager {};
class ExecutionEngine {
public:
    explicit ExecutionEngine(std::unique_ptr<Module> m) : mod_(std::move(m)) {}
    IntFunc getFunctionAddress(const std::string&) { return mod_->func; }
private:
    std::unique_ptr<Module> mod_;
};
class EngineBuilder {
public:
    explicit EngineBuilder(std::unique_ptr<Module> m) : mod_(std::move(m)) {}
    void setMCJITMemoryManager(std::unique_ptr<SectionMemoryManager> m) { mem_ = std::move(m); }
    void setOptLevel(int) {}
    ExecutionEngine* create() { return new ExecutionEngine(std::move(mod_)); }
private:
    std::unique_ptr<Module> mod_;
    std::unique_ptr<SectionMemoryManager> mem_;
};
struct ModuleAnalysisManager {};
class ModulePassManager {
public:
    explicit ModulePassManager(int level) : level_(level) {}
    void run(Module& m, ModuleAnalysisManager&) {
        if (level_ >= 3) m.func = optimized_body;
    }
private:
    int level_;
};
class PassBuilder {
public:
    ModulePassManager buildPerModuleDefaultPipeline(int level) const { return ModulePassManager(level); }
    void registerModuleAnalyses(ModuleAnalysisManager&) const {}
};
static std::unique_ptr<ExecutionEngine> EE;
static bool llvmInit = false;
inline void InitializeLLVM() {
    if (!llvmInit) {
        llvmInit = true;
    }
}

template<typename Function>
Function parseFile(const std::string&, const std::string& name) {
    InitializeLLVM();
    CodeGenAction action;
    std::unique_ptr<Module> module = action.takeModule();
    if (!module) {
        throw std::runtime_error("Failed to retrieve module");
    }
    PassBuilder passBuilder;
    ModulePassManager mpm = passBuilder.buildPerModuleDefaultPipeline(3);
    ModuleAnalysisManager mam;
    passBuilder.registerModuleAnalyses(mam);
    mpm.run(*module, mam);
    EngineBuilder builder(std::move(module));
    builder.setMCJITMemoryManager(std::make_unique<SectionMemoryManager>());
    builder.setOptLevel(3);
    EE.reset(builder.create());
    if (!EE) {
        throw std::runtime_error("Failed to compile file when retrieving the module");
    }
    return reinterpret_cast<Function>(EE->getFunctionAddress(name));
}

} // namespace demo

template demo::IntFunc demo::parseFile<demo::IntFunc>(const std::string&, const std::string&);

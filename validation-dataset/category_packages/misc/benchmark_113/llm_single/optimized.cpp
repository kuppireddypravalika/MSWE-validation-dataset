#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>

namespace demo {

using IntFunc = uint64_t(*)(int);

uint64_t unoptimized_body(int n) {
    // Utilize arithmetic sum formula for optimization
    return static_cast<uint64_t>(n) * (n - 1) / 2 * n;
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
    auto module = action.takeModule();
    EngineBuilder builder(std::move(module));
    builder.setMCJITMemoryManager(std::make_unique<SectionMemoryManager>());
    builder.setOptLevel(3);
    EE.reset(builder.create());
    if (!EE) {
        throw std::runtime_error("Failed to compile file");
    }
    return reinterpret_cast<Function>(EE->getFunctionAddress(name));
}

} // namespace demo

template demo::IntFunc demo::parseFile<demo::IntFunc>(const std::string&, const std::string&);

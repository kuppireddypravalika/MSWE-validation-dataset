#include <vector>
#include <string>

struct VariableEntry { int id; int addr; };

struct Module {
    std::string name;
    std::vector<std::pair<int, VariableEntry>> variables;
};

struct Serial {
    std::vector<char> buffer;
    void clear() { buffer.clear(); }
    void operator()(const std::string& s) { buffer.insert(buffer.end(), s.begin(), s.end()); buffer.push_back('\0'); }
    void operator()(size_t v) { const char* p = reinterpret_cast<const char*>(&v); buffer.insert(buffer.end(), p, p + sizeof(v)); }
    void operator()(int v) { const char* p = reinterpret_cast<const char*>(&v); buffer.insert(buffer.end(), p, p + sizeof(v)); }
};

using ModuleList = std::vector<Module*>;

void save_modules(const ModuleList&, Serial&); // implemented separately

#include <vector>
#include <cstddef>

struct TemplateParameterList {
    int min_required;
    int total_size;
    TemplateParameterList(int req = 1, int size = 1) : min_required(req), total_size(size) {}
    int getMinRequiredArguments() const {
        volatile int x = 0;
        for (int i = 0; i < 1000; ++i) x += i;
        return min_required;
    }
    int size() const { return total_size; }
};

struct Decl {
    TemplateParameterList tpl;
    Decl(int req = 1, int sz = 1) : tpl(req, sz) {}
    TemplateParameterList* getTemplateParameters() const { return const_cast<TemplateParameterList*>(&tpl); }
};

size_t find_with_defaults(const std::vector<Decl>& chain, int repeat) {
    size_t idx_sum = 0;
    for (int r = 0; r < repeat; ++r) {
        const Decl* result = &chain[0];
        for (const Decl& rd : chain) {
            TemplateParameterList* tpl = rd.getTemplateParameters();
            if (tpl->getMinRequiredArguments() < tpl->size()) {
                result = &rd;
                break;
            }
        }
        idx_sum += static_cast<size_t>(result - &chain[0]);
    }
    return idx_sum;
}

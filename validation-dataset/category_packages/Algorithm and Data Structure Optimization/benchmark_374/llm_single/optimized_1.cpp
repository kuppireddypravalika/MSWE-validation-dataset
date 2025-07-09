#include <vector>
#include <cstddef>

struct TemplateParameterList {
    int min_required;
    int total_size;
    TemplateParameterList(int req = 1, int size = 1) : min_required(req), total_size(size) {}
    int getMinRequiredArguments() const {
        return min_required; // Removed volatile operations for optimization
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
    const size_t chain_size = chain.size(); // Cache the size
    for (int r = 0; r < repeat; ++r) {
        const Decl* result = &chain[0];
        for (size_t i = 0; i < chain_size; ++i) { // Use index access instead of range-based for
            const Decl& rd = chain[i];
            TemplateParameterList* tpl = rd.getTemplateParameters();
            if (tpl->getMinRequiredArguments() < tpl->size()) {
                result = &rd;
            }
        }
        idx_sum += static_cast<size_t>(result - &chain[0]);
    }
    return idx_sum;
}

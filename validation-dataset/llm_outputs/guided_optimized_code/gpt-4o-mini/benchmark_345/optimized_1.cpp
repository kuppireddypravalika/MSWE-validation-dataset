#include <iostream>
#include <string>
#include <vector>
#include <memory>

static volatile size_t dummy_global = 0;

inline void mmgc_mem_type(const char* name) {
    size_t sum = 0;
    for (int i = 0; i < 500; ++i) {
        for (const char* c = name; *c != '\0'; ++c) {
            sum += *c;
        }
    }
    dummy_global += sum;
}

#define MMGC_MEM_TYPE(x) mmgc_mem_type(x)

struct Page { char data[4096]; };
struct Stats { int32_t pages = 0; };

class Fragmento {
    std::vector<std::unique_ptr<Page>> _freePages;
    Stats _stats;
    int32_t _max_pages;
public:
    Fragmento(int32_t max_pages) : _max_pages(max_pages) {}
    void releasePages() {
        _freePages.clear();
        _stats.pages = 0;
    }
    void pagesGrow(int32_t count) {
        MMGC_MEM_TYPE("NanojitFragmentoMem");
        if (_stats.pages < _max_pages) {
            int32_t new_page_count = std::min(count, _max_pages - _stats.pages);
            if (new_page_count > 0) {
                MMGC_MEM_TYPE("NanoJitMem");
                _freePages.reserve(_freePages.size() + new_page_count);
                for (int i = 0; i < new_page_count; ++i) {
                    _freePages.emplace_back(std::make_unique<Page>());
                }
                _stats.pages += new_page_count;
            }
        }
    }
    size_t pageCount() const { return _freePages.size(); }
};

size_t run_benchmark(int repeat) {
    Fragmento f(1000000);
    for (int i = 0; i < repeat; ++i) {
        f.pagesGrow(50);
        f.releasePages();
    }
    return f.pageCount();
}

// Explicit template instantiation statements (if any) would go here.
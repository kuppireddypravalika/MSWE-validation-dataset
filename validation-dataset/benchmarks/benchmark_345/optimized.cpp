#include <iostream>
#include <string>
#include <vector>
#include <cstring>

static volatile size_t dummy_global = 0;

inline void mmgc_mem_tag(const char* name) {
    dummy_global += strlen(name);
}

#define MMGC_MEM_TAG(x) mmgc_mem_tag(x)

struct Page { char data[4096]; };
struct Stats { int32_t pages = 0; };

class Fragmento {
    std::vector<Page*> _freePages;
    Stats _stats;
    int32_t _max_pages;
public:
    Fragmento(int32_t max_pages) : _max_pages(max_pages) {}
    ~Fragmento() { releasePages(); }
    void releasePages() {
        for (Page* p : _freePages) delete p;
        _freePages.clear();
        _stats.pages = 0;
    }
    void pagesGrow(int32_t count) {
        MMGC_MEM_TAG("NanojitFragmentoMem");
        if (_stats.pages < _max_pages) {
            if (_stats.pages + count > _max_pages)
                count = _max_pages - _stats.pages;
            if (count < 0) count = 0;
            MMGC_MEM_TAG("NanoJitMem");
            for (int i = 0; i < count; ++i) {
                Page* memory = new Page;
                _freePages.push_back(memory);
            }
            _stats.pages += count;
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

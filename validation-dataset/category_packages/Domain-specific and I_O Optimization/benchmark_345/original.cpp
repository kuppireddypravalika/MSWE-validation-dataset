#include <iostream>
#include <string>
#include <vector>
#include <cstring>

static volatile size_t dummy_global = 0;

inline void mmgc_mem_type(const char* name) {
    std::string tmp;
    for (int i = 0; i < 500; ++i) {
        tmp += name;
    }
    size_t sum = 0;
    for (char c : tmp) sum += c;
    dummy_global += sum;
}

#define MMGC_MEM_TYPE(x) mmgc_mem_type(x)

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
        MMGC_MEM_TYPE("NanojitFragmentoMem");
        if (_stats.pages < _max_pages) {
            if (_stats.pages + count > _max_pages)
                count = _max_pages - _stats.pages;
            if (count < 0) count = 0;
            MMGC_MEM_TYPE("NanoJitMem");
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

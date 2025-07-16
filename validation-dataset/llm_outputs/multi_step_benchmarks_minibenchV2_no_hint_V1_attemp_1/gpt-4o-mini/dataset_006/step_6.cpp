#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm> // Needed for std::swap and std::snprintf

enum { SORT_A_TO_Z = 0, SORT_Z_TO_A = 1 };

struct RomSelection {
    char name[128];
    char fullpath[256];
    int id;
    int size;
    RomSelection* next;
};

static RomSelection* list = nullptr;
static int gSortOrder = SORT_A_TO_Z;

static void swap_nodes(RomSelection* a, RomSelection* b) {
    std::swap(a->id, b->id);
    std::swap(a->size, b->size);
    std::swap_ranges(std::begin(a->name), std::end(a->name), std::begin(b->name));
    std::swap_ranges(std::begin(a->fullpath), std::end(a->fullpath), std::begin(b->fullpath));
}

static void sort_list(RomSelection* start, int order) {
    if (!start) return;
    int swapped;
    RomSelection* ptr1;
    RomSelection* lptr = nullptr;
    do {
        swapped = 0;
        ptr1 = start;
        while (ptr1->next != lptr) {
            RomSelection* nextPtr = ptr1->next;
            bool cond = (order == SORT_Z_TO_A) ? (std::strcmp(ptr1->name, nextPtr->name) < 0) : (std::strcmp(ptr1->name, nextPtr->name) > 0);
            if (cond) {
                swap_nodes(ptr1, nextPtr);
                swapped = 1;
            }
            ptr1 = nextPtr;
        }
        lptr = ptr1;
    } while (swapped);
}

static void ensure_list(int count) {
    if (list) return;
    static std::vector<RomSelection> storage;
    storage.resize(count);
    for (int i = 0; i < count; i++) {
        std::snprintf(storage[i].name, sizeof(storage[i].name), "%07d", rand() % 100000);
        std::snprintf(storage[i].fullpath, sizeof(storage[i].fullpath), "rom_%05d", i);
        storage[i].id = i;
        storage[i].size = i;
        storage[i].next = (i + 1 < count) ? &storage[i + 1] : nullptr;
    }
    list = &storage[0];
}

size_t draw_loop(int loops) {
    const int roms = 15000;
    ensure_list(roms);

    for (int i = 0; i < loops; i++) {
        if (i == loops / 2) gSortOrder = SORT_Z_TO_A; // change order once
        sort_list(list, gSortOrder);
    }

    size_t sum = 0;
    RomSelection* p = list;
    while (p) {
        sum += static_cast<unsigned char>(p->name[0]);
        p = p->next;
    }
    return sum;
}

// Explicit instantiations
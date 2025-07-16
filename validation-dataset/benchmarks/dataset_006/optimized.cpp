#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

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
static int oldSortOrder = -1;

static void swap_nodes(RomSelection* a, RomSelection* b) {
    char nametmp[128], pathtmp[256];
    sprintf(nametmp, "%s", a->name);
    sprintf(pathtmp, "%s", a->fullpath);
    int idtmp = a->id;
    int sizetmp = a->size;

    sprintf(a->name, "%s", b->name);
    sprintf(a->fullpath, "%s", b->fullpath);
    a->id = b->id;
    a->size = b->size;

    sprintf(b->name, "%s", nametmp);
    sprintf(b->fullpath, "%s", pathtmp);
    b->id = idtmp;
    b->size = sizetmp;
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
            bool cond = false;
            switch(order) {
                case SORT_Z_TO_A:
                    cond = std::strcmp(ptr1->name, ptr1->next->name) < 0;
                    break;
                case SORT_A_TO_Z:
                    cond = std::strcmp(ptr1->name, ptr1->next->name) > 0;
                    break;
                default: break;
            }
            if (cond) {
                swap_nodes(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

static void ensure_list(int count) {
    if (list) return;
    static std::vector<RomSelection> storage;
    storage.resize(count);
    for (int i=0;i<count;i++) {
        sprintf(storage[i].name, "%07d", rand()%100000);
        sprintf(storage[i].fullpath, "rom_%05d", i);
        storage[i].id = i;
        storage[i].size = i;
        storage[i].next = (i+1<count)? &storage[i+1] : nullptr;
    }
    list = &storage[0];
}

size_t draw_loop(int loops) {
    const int roms = 15000;
    ensure_list(roms);

    for (int i=0;i<loops;i++) {
        if (i == loops/2) gSortOrder = SORT_Z_TO_A; // change order once
        if (oldSortOrder != gSortOrder) {
            oldSortOrder = gSortOrder;
            sort_list(list, gSortOrder);
        }
    }

    size_t sum=0; RomSelection* p=list; while(p){ sum += static_cast<unsigned char>(p->name[0]); p=p->next; }
    return sum;
}


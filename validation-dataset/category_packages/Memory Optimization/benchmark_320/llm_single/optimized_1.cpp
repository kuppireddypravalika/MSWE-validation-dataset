#include <vector>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <algorithm>

struct Point {
    uint16_t x, y;
    int16_t z;
    int16_t height;
};

struct Item {
    uint16_t x, y;
    int16_t z;
    int16_t height;
    uint32_t serial;
};

struct Shape {
    int16_t z;
    int16_t height;
};

struct LosCache {
    uint16_t last_x;
    uint16_t last_y;
    std::vector<Shape> shapes;
    std::vector<const Item*> dyn_items;
};

static std::vector<Item> g_items(1000);
static std::vector<Shape> g_shapes(1000);

static void init_world() {
    for (size_t i = 0; i < g_items.size(); ++i) {
        g_items[i] = { static_cast<uint16_t>(rand() % 50), static_cast<uint16_t>(rand() % 50), static_cast<int16_t>(rand() % 20), 3, static_cast<uint32_t>(i + 1) };
    }
    for (size_t i = 0; i < g_shapes.size(); ++i) {
        g_shapes[i] = { static_cast<int16_t>(rand() % 20), static_cast<int16_t>(1 + rand() % 5) };
    }
}

static bool dynamic_item_blocks_los(const Point& att, const Point& target, uint16_t x, uint16_t y, int16_t z, LosCache& cache) {
    for (const Item* item : cache.dyn_items) {
        if (item->x == x && item->y == y && item->z <= z && z < item->z + item->height) {
            return true;
        }
    }
    return false;
}

static bool static_item_blocks_los(uint16_t x, uint16_t y, int16_t z, LosCache& cache) {
    if (cache.last_x != x || cache.last_y != y) {
        cache.last_x = x;
        cache.last_y = y;
        cache.shapes.clear();
        for (size_t i = 0; i < g_shapes.size(); ++i) {
            if ((i + x + y) % 7 == 0)
                cache.shapes.push_back(g_shapes[i]);
        }
    }
    for (const auto& shape : cache.shapes) {
        int ob_z = shape.z;
        int ob_ht = shape.height == 0 ? 1 : shape.height;
        if (ob_z <= z && z < ob_z + ob_ht)
            return true;
    }
    return false;
}

static bool los_blocked(const Point& att, const Point& target, uint16_t x, uint16_t y, int16_t z, LosCache& cache) {
    if ((att.x == x && att.y == y && att.z <= z && z <= att.z + att.height) ||
        (target.x == x && target.y == y && target.z <= z && z <= target.z + target.height))
        return false;
    return dynamic_item_blocks_los(att, target, x, y, z, cache) || static_item_blocks_los(x, y, z, cache);
}

static bool has_los(const Point& att, const Point& tgt) {
    LosCache cache{0xFFFF, 0xFFFF};
    cache.shapes.reserve(g_shapes.size());
    cache.dyn_items.reserve(g_items.size());

    for (const auto& item : g_items) {
        if (std::abs(static_cast<int>(item.x) - static_cast<int>(att.x)) < 2 &&
            std::abs(static_cast<int>(item.y) - static_cast<int>(att.y)) < 2)
            cache.dyn_items.push_back(&item);
    }

    int x1 = std::min(att.x, tgt.x);
    int y1 = std::min(att.y, tgt.y);
    int z1 = std::max(att.z + att.height, tgt.z + tgt.height);
    int x2 = std::max(att.x, tgt.x);
    int y2 = std::max(att.y, tgt.y);
    int z2 = std::min(att.z + att.height, tgt.z + tgt.height);

    int dx = x2 - x1;
    int dy = y2 - y1;
    int dz = z2 - z1;
    if (std::abs(dx) > 20 || std::abs(dy) > 20)
        return false;

    if (dx == 0 && dy == 0) {
        return (z1 >= z2);
    }

    int ax = std::abs(dx) * 2;
    int ay = std::abs(dy) * 2;
    int az = std::abs(dz) * 2;
    int sx = dx != 0 ? (dx > 0 ? 1 : -1) : 0;
    int sy = dy != 0 ? (dy > 0 ? 1 : -1) : 0;
    int sz = dz != 0 ? (dz > 0 ? 1 : -1) : 0;

    if (ax >= ay && ax >= az) {
        int yd = ay - (ax >> 1);
        int zd = az - (ax >> 1);
        for (int x = x1, y = y1, z = z1; x != x2; x += sx) {
            if (los_blocked(att, tgt, x, y, z, cache)) return false;
            if (yd >= 0) { y += sy; yd -= ax; }
            if (zd >= 0) { z += sz; zd -= ax; }
            yd += ay;
            zd += az;
        }
    } else if (ay >= ax && ay >= az) {
        int xd = ax - (ay >> 1);
        int zd = az - (ay >> 1);
        for (int x = x1, y = y1, z = z1; y != y2; y += sy) {
            if (los_blocked(att, tgt, x, y, z, cache)) return false;
            if (xd >= 0) { x += sx; xd -= ay; }
            if (zd >= 0) { z += sz; zd -= ay; }
            xd += ax;
            zd += az;
        }
    } else {
        int xd = ax - (az >> 1);
        int yd = ay - (az >> 1);
        for (int x = x1, y = y1, z = z1; z != z2; z += sz) {
            if (los_blocked(att, tgt, x, y, z, cache)) return false;
            if (xd >= 0) { x += sx; xd -= az; }
            if (yd >= 0) { y += sy; yd -= az; }
            xd += ax;
            yd += ay;
        }
    }
    return true;
}

uint64_t run(size_t iterations) {
    static bool initialized = false;
    if (!initialized) { init_world(); initialized = true; }
    Point att{10,10,0,5};
    Point tgt{20,20,0,5};
    uint64_t sum = 0;
    for (size_t i = 0; i < iterations; ++i) {
        sum += has_los(att, tgt);
        att.x = (att.x + 1) % 50;
        tgt.y = (tgt.y + 1) % 50;
    }
    return sum;
}

// Explicit template instantiation no templates were used.

#include <vector>
#include <cstdint>
#include <cstdlib>
#include <cmath>

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

static std::vector<Item> g_items;
static std::vector<Shape> g_shapes;

static void init_world() {
    g_items.resize(1000);
    for (size_t i = 0; i < g_items.size(); ++i) {
        g_items[i].x = rand() % 50;
        g_items[i].y = rand() % 50;
        g_items[i].z = rand() % 20;
        g_items[i].height = 3;
        g_items[i].serial = static_cast<uint32_t>(i + 1);
    }
    g_shapes.resize(1000);
    for (size_t i = 0; i < g_shapes.size(); ++i) {
        g_shapes[i].z = rand() % 20;
        g_shapes[i].height = 1 + rand() % 5;
    }
}

static bool dynamic_item_blocks_los(const Point& att, const Point& target,
                                    uint16_t x, uint16_t y, int16_t z,
                                    LosCache& cache) {
    for (const Item* item : cache.dyn_items) {
        if (item->x == x && item->y == y) {
            if (item->z <= z && z < item->z + item->height)
                return true;
        }
    }
    return false;
}

static bool static_item_blocks_los(uint16_t x, uint16_t y, int16_t z,
                                   LosCache& cache) {
    if (cache.last_x != x || cache.last_y != y) {
        cache.shapes.clear();
        cache.last_x = x;
        cache.last_y = y;
        for (size_t i = 0; i < g_shapes.size(); ++i) {
            if ((i + x + y) % 7 == 0)
                cache.shapes.push_back(g_shapes[i]);
        }
    }
    for (const auto& shape : cache.shapes) {
        int ob_ht = shape.height;
        int ob_z = shape.z;
        if (ob_ht == 0) {
            --ob_z;
            ++ob_ht;
        }
        if (ob_z <= z && z < ob_z + ob_ht)
            return true;
    }
    return false;
}

static bool los_blocked(const Point& att, const Point& target,
                        uint16_t x, uint16_t y, int16_t z, LosCache& cache) {
    if (att.x == x && att.y == y && att.z <= z && z <= att.z + att.height)
        return false;
    if (target.x == x && target.y == y && target.z <= z &&
        z <= target.z + target.height)
        return false;
    return dynamic_item_blocks_los(att, target, x, y, z, cache) ||
           static_item_blocks_los(x, y, z, cache);
}

static bool has_los(const Point& att, const Point& tgt) {
    static thread_local LosCache cache;
    cache.last_x = 0xFFFF;
    cache.last_y = 0xFFFF;
    cache.shapes.clear();
    cache.dyn_items.clear();
    for (const auto& item : g_items) {
        if (std::abs((int)item.x - (int)att.x) < 2 &&
            std::abs((int)item.y - (int)att.y) < 2)
            cache.dyn_items.push_back(&item);
    }

    int x1, y1, z1;
    int x2, y2, z2;
    int xd, yd, zd;
    int x, y, z;
    int ax, ay, az;
    int sx, sy, sz;
    int dx, dy, dz;

    int att_look_height = att.height;
    int tgt_look_height = tgt.height;

    if ((att.y < tgt.y) || (att.y == tgt.y && att.z < tgt.z)) {
        x1 = att.x;
        y1 = att.y;
        z1 = att.z + att_look_height;
        x2 = tgt.x;
        y2 = tgt.y;
        z2 = tgt.z + tgt_look_height;
    } else {
        x1 = tgt.x;
        y1 = tgt.y;
        z1 = tgt.z + tgt_look_height;
        x2 = att.x;
        y2 = att.y;
        z2 = att.z + att_look_height;
    }

    dx = x2 - x1;
    dy = y2 - y1;
    dz = z2 - z1;
    const int los_range = 20;
    if (std::abs(dx) > los_range || std::abs(dy) > los_range)
        return false;
    if (!dx && !dy) {
        if (!dz) return true;
        if (att.z <= tgt.z && tgt.z <= att.z + att.height) return true;
        if (att.z <= tgt.z + tgt_look_height &&
            tgt.z + tgt_look_height <= att.z + att.height)
            return true;
    }
    ax = std::abs(dx) << 1;
    ay = std::abs(dy) << 1;
    az = std::abs(dz) << 1;
    sx = (dx < 0) ? -1 : (dx > 0);
    sy = (dy < 0) ? -1 : (dy > 0);
    sz = (dz < 0) ? -1 : (dz > 0);
    x = x1;
    y = y1;
    z = z1;
    if (ax >= ay && ax >= az) {
        yd = ay - (ax >> 1);
        zd = az - (ax >> 1);
        for (;;) {
            if (los_blocked(att, tgt, x, y, z, cache)) return false;
            if (x == x2) return true;
            if (yd >= 0) { y += sy; yd -= ax; }
            if (zd >= 0) { z += sz; zd -= ax; }
            x += sx;
            yd += ay;
            zd += az;
        }
    } else if (ay >= ax && ay >= az) {
        xd = ax - (ay >> 1);
        zd = az - (ay >> 1);
        for (;;) {
            if (los_blocked(att, tgt, x, y, z, cache)) return false;
            if (y == y2) return true;
            if (xd >= 0) { x += sx; xd -= ay; }
            if (zd >= 0) { z += sz; zd -= ay; }
            y += sy;
            xd += ax;
            zd += az;
        }
    } else {
        xd = ax - (az >> 1);
        yd = ay - (az >> 1);
        for (;;) {
            if (los_blocked(att, tgt, x, y, z, cache)) return false;
            if (z == z2) return true;
            if (xd >= 0) { x += sx; xd -= az; }
            if (yd >= 0) { y += sy; yd -= az; }
            z += sz;
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

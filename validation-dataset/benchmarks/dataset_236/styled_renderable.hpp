#pragma once
#include <cstddef>

namespace ImGuiSim {
inline volatile int sink = 0;
inline bool IsItemActivated() {
    int sum = 0;
    for (int i = 0; i < 10000; ++i) sum += (i + sink) % 3;
    sink += sum;
    return sum & 1;
}
inline bool IsItemDeactivated() {
    int sum = 0;
    for (int i = 0; i < 10000; ++i) sum += (i + sink) % 5;
    sink += sum;
    return sum & 1;
}
}

using Callback = void(*)(int&);

struct EventManager {
    int counter = 0;
    void Call(Callback cb, int handle) {
        if (cb) cb(counter);
    }
};

struct StyledRenderable {
    EventManager* Manager = nullptr;
    Callback OnActivate = nullptr;
    Callback OnDeactivate = nullptr;
    int Handle = 0;
};

void render_item(StyledRenderable& item);

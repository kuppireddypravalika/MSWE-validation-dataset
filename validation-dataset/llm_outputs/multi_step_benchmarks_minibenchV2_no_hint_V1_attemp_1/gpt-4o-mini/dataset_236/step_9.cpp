// optimized.cpp
#include "styled_renderable.hpp"

void render_item(StyledRenderable& item) {
    // Store activation and deactivation state once per call
    bool activated = ImGuiSim::IsItemActivated();
    bool deactivated = ImGuiSim::IsItemDeactivated();

    // Call the OnActivate callback if activated
    if (activated && item.OnActivate) {
        item.Manager->Call(item.OnActivate, item.Handle);
    }
    // Call the OnDeactivate callback if deactivated
    if (deactivated && item.OnDeactivate) {
        item.Manager->Call(item.OnDeactivate, item.Handle);
    }
}

// Explicit template instantiations
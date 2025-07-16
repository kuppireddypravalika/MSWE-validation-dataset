// original.cpp
#include "styled_renderable.hpp"

void render_item(StyledRenderable& item) {
    // Store the activation and deactivation state only once per invocation
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
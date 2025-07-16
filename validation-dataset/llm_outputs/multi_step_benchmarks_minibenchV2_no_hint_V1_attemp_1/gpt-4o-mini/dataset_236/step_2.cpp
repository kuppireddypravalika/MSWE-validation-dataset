#include "styled_renderable.hpp"

void render_item(StyledRenderable& item) {
    // Store the state of item activation and deactivation
    bool activated = ImGuiSim::IsItemActivated();
    bool deactivated = ImGuiSim::IsItemDeactivated();

    // Use the stored state to call callbacks
    if (activated && item.OnActivate) {
        item.Manager->Call(item.OnActivate, item.Handle);
    }
    if (deactivated && item.OnDeactivate) {
        item.Manager->Call(item.OnDeactivate, item.Handle);
    }
}

// Explicit template instantiations

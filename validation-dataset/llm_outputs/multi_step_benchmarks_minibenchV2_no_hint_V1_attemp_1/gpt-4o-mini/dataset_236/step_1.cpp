#include "styled_renderable.hpp"

void render_item(StyledRenderable& item) {
    // Store the results of IsItemActivated and IsItemDeactivated to avoid repetitive calls.
    bool activated = ImGuiSim::IsItemActivated();
    bool deactivated = ImGuiSim::IsItemDeactivated();
    
    // Only check for activation if it has been activated.
    if (activated && item.OnActivate) {
        item.Manager->Call(item.OnActivate, item.Handle);
    }
    // Only check for deactivation if it has been deactivated.
    if (deactivated && item.OnDeactivate) {
        item.Manager->Call(item.OnDeactivate, item.Handle);
    }
}


// Explicit template instantiations

#include "styled_renderable.hpp"

void render_item(StyledRenderable& item) {
    if (ImGuiSim::IsItemActivated() && item.OnActivate) {
        item.Manager->Call(item.OnActivate, item.Handle);
    }
    if (ImGuiSim::IsItemDeactivated() && item.OnDeactivate) {
        item.Manager->Call(item.OnDeactivate, item.Handle);
    }
}

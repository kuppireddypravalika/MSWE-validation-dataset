#include "styled_renderable.hpp"

void render_item(StyledRenderable& item) {
    if (item.OnActivate && ImGuiSim::IsItemActivated()) {
        item.Manager->Call(item.OnActivate, item.Handle);
    }
    if (item.OnDeactivate && ImGuiSim::IsItemDeactivated()) {
        item.Manager->Call(item.OnDeactivate, item.Handle);
    }
}

# Dataset 236: IMGUI activation check order

Derived from [BG3Extender](https://github.com/Norbyte/bg3se) commit `fc4633a0`. The patch moves the `OnActivate`/`OnDeactivate` null checks before calling `ImGui::IsItemActivated` and `ImGui::IsItemDeactivated`. This avoids executing the expensive ImGui queries when no handler is registered.

This benchmark models a simple event manager and expensive activation checks to measure the performance impact of the condition order change.

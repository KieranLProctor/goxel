#pragma once

#include "input_event.h"
#include "layer.h"

namespace goxel::ui
{

class OverlayLayer : public core::Layer
{
  public:
    OverlayLayer();
    ~OverlayLayer() override = default;

    auto on_event(core::Event &event) -> void override;

    auto on_update(float time_step) -> void override;
    auto on_render() -> void override;

  private:
    bool m_show_menu_bar = false;

    static auto show_menu_bar(bool is_visible) -> void;

    static auto on_mouse_button_input(const core::MouseButtonPressedEvent &event) -> bool;
    auto on_keyboard_input(const core::KeyPressedEvent &event) -> bool;
};

} // namespace ui

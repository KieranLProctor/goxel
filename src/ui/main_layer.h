#pragma once

#include "ecs.h"
#include "input_event.h"
#include "layer.h"
#include "systems/render_system.h"
#include <entt/entt.hpp>

namespace goxel::ui
{

class MainLayer : public core::Layer
{
  public:
    MainLayer();
    ~MainLayer() override;

    auto on_update(float time_step) -> void override;
    auto on_render() -> void override;
    auto on_event(core::Event &event) -> void override;

    auto on_keyboard_input(const core::KeyPressedEvent &event) -> bool;
    // auto on_mouse_input(core::MouseButtonEvent &event) -> bool;
    auto on_mouse_move(const core::MouseMovedEvent &event) const -> bool;

  private:
    game::systems::RenderSystem m_render_system;

    entt::registry &m_registry = game::get_registry();
    entt::entity m_player = entt::null;

    float m_move_speed = 1.0f;
    float m_mouse_sensitivity = 1.0f;
    bool m_cursor_captured = false;
    bool m_render_wireframe = false;

    static constexpr float k_base_sensitivity = 0.1f;
};

} // namespace ui

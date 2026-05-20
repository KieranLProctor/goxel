#pragma once

#include "../game/camera.h"
#include "event.h"
#include "glad/glad.h"
#include "input_event.h"
#include "layer.h"

namespace ui
{

class MainLayer : public core::Layer
{
  public:
    MainLayer();
    ~MainLayer() override;

    auto on_update(float time_step) -> void override;
    auto on_render() -> void override;
    auto on_event(core::Event &event) -> void override;

    auto on_keyboard_input(core::KeyPressedEvent &event) -> bool;
    // auto on_mouse_input(core::MouseButtonEvent &event) -> bool;
    auto on_mouse_move(core::MouseMovedEvent &event) -> bool;

  private:
    GLuint m_simple_shader = 0;
    GLuint m_simple_vao = 0;
    GLuint m_simple_vbo = 0;

    game::Camera m_camera {-90.0f, 0.0f};

    float m_move_speed = 1.0f;
    static constexpr float k_base_sensitivity = 0.1f;
    float m_mouse_sensitivity = 1.0f;
    bool m_cursor_captured = false;
};

} // namespace ui

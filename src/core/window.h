#pragma once

#include "event.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <functional>
#include <string>

#include <glm/glm.hpp>

namespace core
{

struct WindowSpec
{
    std::string title;
    int width;
    int height;
    bool is_resizable;
    bool is_vsync;

    std::function<void(Event &)> event_callback;
};

class Window
{
  public:
    explicit Window(WindowSpec spec);
    ~Window();

    auto create() -> void;
    auto destroy() -> void;
    auto update() const -> void;

    auto get_framebuffer_size() const -> glm::vec2;
    auto get_mouse_position() const -> glm::vec2;

    auto should_close() const -> bool;

    auto get_handle() const -> GLFWwindow *;
    auto init_callbacks() const -> void;

  private:
    static auto glfw_error_callback(int error, const char *description) -> void;
    auto raise_event(Event &event) const -> void;

    WindowSpec m_spec;
    GLFWwindow *m_handle = nullptr;
};

} // namespace core

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
    Window(WindowSpec spec);
    ~Window();

    auto create() -> void;
    auto destroy() -> void;

    auto update() -> void;

    auto raise_event(Event &event) -> void;

    auto get_framebuffer_size() -> glm::vec2;
    auto get_mouse_position() -> glm::vec2;

    auto should_close() -> bool;

    auto get_handle() -> GLFWwindow *;

  private:
    WindowSpec m_spec;
    GLFWwindow *m_handle = nullptr;
};

} // namespace core

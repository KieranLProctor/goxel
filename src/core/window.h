#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>

#include <glm/glm.hpp>

namespace core
{

enum class WindowMode
{
    FULLSCREEN,
    WINDOWED,
};

class Window
{
  public:
    Window(WindowMode mode, std::string title, std::uint32_t width, std::uint32_t height, bool is_resizable,
           bool is_vsync);
    ~Window();

    auto create() -> void;
    auto destroy() -> void;

    auto update() -> void;

    auto get_framebuffer_size() -> glm::vec2;
    auto get_mouse_position() -> glm::vec2;

    auto get_should_close() -> bool;

    auto get_window() -> GLFWwindow *;

  private:
    WindowMode m_mode;
    std::string m_title;
    std::uint32_t m_width;
    std::uint32_t m_height;
    bool m_is_resizable;
    bool m_is_vsync;

    GLFWwindow *m_window = nullptr;
};

} // namespace core

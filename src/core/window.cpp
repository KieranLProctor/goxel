#include "window.h"

#include <iostream>
#include <utility>

namespace core
{

Window::Window(WindowMode mode, std::string title, std::uint32_t width, std::uint32_t height, bool is_resizable,
               bool is_vsync)
{
    m_mode = mode;
    m_title = std::move(title);
    m_width = width;
    m_height = height;
    m_is_resizable = is_resizable;
    m_is_vsync = is_vsync;
}

Window::~Window()
{
    destroy();
}

auto Window::create() -> void
{
    // TODO: This should be conditional based on OS.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, m_is_resizable ? GLFW_TRUE : GLFW_FALSE);

    m_window =
        glfwCreateWindow(static_cast<int>(m_width), static_cast<int>(m_height), m_title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        assert(false);

        return;
    }

    glfwMakeContextCurrent(m_window);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    glfwSwapInterval(m_is_vsync ? 1 : 0);

    glfwSetWindowUserPointer(m_window, this);
}

auto Window::destroy() -> void
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
    }

    m_window = nullptr;
}

auto Window::update() -> void
{
    glfwSwapBuffers(m_window);
}

auto Window::get_framebuffer_size() -> glm::vec2
{
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);

    return {width, height};
}

auto Window::get_mouse_position() -> glm::vec2
{
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);

    return {static_cast<float>(x), static_cast<float>(y)};
}

auto Window::get_should_close() -> bool
{
    return glfwWindowShouldClose(m_window);
}

auto Window::get_window() -> GLFWwindow *
{
    return m_window;
}

} // namespace core

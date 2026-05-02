#include "window.h"

#include "window_event.h"

#include <utility>

#include "spdlog/spdlog.h"

namespace core
{

Window::Window(WindowSpec spec) : m_spec(spec) {}

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
    glfwWindowHint(GLFW_RESIZABLE, m_spec.is_resizable ? GLFW_TRUE : GLFW_FALSE);

    if (!glfwInit())
    {
        spdlog::error("Failed to initialise GLFW!");
        assert(false);
    }

    m_handle = glfwCreateWindow(m_spec.width, m_spec.height, m_spec.title.c_str(), nullptr, nullptr);
    if (!m_handle)
    {
        spdlog::error("Failed to create GLFW window!");
        assert(false);
    }

    glfwMakeContextCurrent(m_handle);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    glfwSwapInterval(m_spec.is_vsync ? 1 : 0);

    glfwSetWindowUserPointer(m_handle, this);

    glfwSetWindowCloseCallback(m_handle,
                               [](GLFWwindow *handle)
                               {
                                   auto window = *static_cast<Window *>(glfwGetWindowUserPointer(handle));
                                   WindowClosedEvent event;
                                   window.raise_event(event);
                               });
}

auto Window::destroy() -> void
{
    if (m_handle)
    {
        glfwDestroyWindow(m_handle);
    }

    m_handle = nullptr;
}

auto Window::update() -> void
{
    glfwSwapBuffers(m_handle);
}

auto Window::get_framebuffer_size() -> glm::vec2
{
    int width, height;
    glfwGetFramebufferSize(m_handle, &width, &height);

    return {width, height};
}

auto Window::get_mouse_position() -> glm::vec2
{
    double x, y;
    glfwGetCursorPos(m_handle, &x, &y);

    return {static_cast<float>(x), static_cast<float>(y)};
}

auto Window::should_close() -> bool
{
    return glfwWindowShouldClose(m_handle) != 0;
}

auto Window::get_handle() -> GLFWwindow *
{
    return m_handle;
}

} // namespace core

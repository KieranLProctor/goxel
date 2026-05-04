#include "window.h"

#include "glad/glad.h"
#include "input_event.h"
#include "window_event.h"

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
                                   Window &window = *static_cast<Window *>(glfwGetWindowUserPointer(handle));

                                   WindowClosedEvent event;
                                   window.raise_event(event);
                               });

    glfwSetWindowSizeCallback(m_handle,
                              [](GLFWwindow *handle, int width, int height)
                              {
                                  Window &window = *static_cast<Window *>(glfwGetWindowUserPointer(handle));

                                  WindowResizedEvent event(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
                                  window.raise_event(event);
                              });

    glfwSetKeyCallback(m_handle,
                       [](GLFWwindow *handle, int key, int scancode, int action, int mods)
                       {
                           Window &window = *static_cast<Window *>(glfwGetWindowUserPointer(handle));

                           switch (action)
                           {
                           case GLFW_PRESS:
                           case GLFW_REPEAT:
                           {
                               KeyPressedEvent event(key, action == GLFW_REPEAT);
                               window.raise_event(event);
                               break;
                           }
                           case GLFW_RELEASE:
                           {
                               KeyReleasedEvent event(key);
                               window.raise_event(event);
                               break;
                           }
                           }
                       });

    glfwSetMouseButtonCallback(m_handle,
                               [](GLFWwindow *handle, int button, int action, int mods)
                               {
                                   Window &window = *static_cast<Window *>(glfwGetWindowUserPointer(handle));

                                   switch (action)
                                   {
                                   case GLFW_PRESS:
                                   {
                                       MouseButtonPressedEvent event(button);
                                       window.raise_event(event);
                                       break;
                                   }
                                   case GLFW_RELEASE:
                                   {
                                       MouseButtonReleasedEvent event(button);
                                       window.raise_event(event);
                                       break;
                                   }
                                   }
                               });

    glfwSetScrollCallback(m_handle,
                          [](GLFWwindow *handle, double xOffset, double yOffset)
                          {
                              Window &window = *static_cast<Window *>(glfwGetWindowUserPointer(handle));

                              MouseScrolledEvent event(xOffset, yOffset);
                              window.raise_event(event);
                          });

    glfwSetCursorPosCallback(m_handle,
                             [](GLFWwindow *handle, double x, double y)
                             {
                                 Window &window = *static_cast<Window *>(glfwGetWindowUserPointer(handle));

                                 MouseMovedEvent event(x, y);
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

auto Window::raise_event(Event &event) -> void
{
    if (m_spec.event_callback)
    {
        m_spec.event_callback(event);
    }
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

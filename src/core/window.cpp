#include "window.h"

#include <utility>

#include "glad/glad.h"
#include "imgui_impl_glfw.h"
#include "input_event.h"
#include "window_event.h"

#include "spdlog/spdlog.h"

namespace goxel::core
{

Window::Window(WindowSpec spec) : m_spec(std::move(spec)) {}

Window::~Window()
{
    destroy();
}

auto Window::create() -> void
{
    glfwSetErrorCallback(glfw_error_callback);

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, m_spec.is_resizable ? GLFW_TRUE : GLFW_FALSE);

    float x_scale = 1.0f;
    float y_scale = 1.0f;
    glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &x_scale, &y_scale);
    float scale = std::max(x_scale, y_scale);

    m_handle = glfwCreateWindow(static_cast<int>(m_spec.width * scale), static_cast<int>(m_spec.height * scale),
                                m_spec.title.c_str(), nullptr, nullptr);
    if (!m_handle)
    {
        spdlog::critical("Failed to create GLFW window!");
        assert(false);
    }

    glfwMakeContextCurrent(m_handle);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        spdlog::critical("Failed to initialize GLAD!");
        assert(false);
    }

    glfwSwapInterval(m_spec.is_vsync ? 1 : 0);

    glfwSetWindowUserPointer(m_handle, this);
}

auto Window::destroy() -> void
{
    if (m_handle)
    {
        glfwDestroyWindow(m_handle);
    }

    m_handle = nullptr;
}

auto Window::update() const -> void
{
    if (!m_handle)
    {
        return;
    }

    glfwSwapBuffers(m_handle);
}

auto Window::raise_event(Event &event) const -> void
{
    if (m_spec.event_callback)
    {
        m_spec.event_callback(event);
    }
}

auto Window::get_framebuffer_size() const -> glm::vec2
{
    if (!m_handle)
    {
        return {0, 0};
    }

    int width;
    int height;

    glfwGetFramebufferSize(m_handle, &width, &height);

    return {width, height};
}

auto Window::get_mouse_position() const -> glm::vec2
{
    if (!m_handle)
    {
        return {0.0f, 0.0f};
    }

    double x;
    double y;
    glfwGetCursorPos(m_handle, &x, &y);

    return {static_cast<float>(x), static_cast<float>(y)};
}

auto Window::set_cursor_captured(bool captured) const -> void
{
    if (!m_handle)
    {
        return;
    }

    glfwSetInputMode(m_handle, GLFW_CURSOR, captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

    if (glfwRawMouseMotionSupported())
    {
        glfwSetInputMode(m_handle, GLFW_RAW_MOUSE_MOTION, captured ? GLFW_TRUE : GLFW_FALSE);
    }
}

auto Window::should_close() const -> bool
{
    return glfwWindowShouldClose(m_handle) != 0;
}

auto Window::get_handle() const -> GLFWwindow *
{
    return m_handle;
}

auto Window::init_callbacks() const -> void
{
    glfwSetWindowCloseCallback(m_handle,
                               [](GLFWwindow *handle)
                               {
                                   const auto *window = static_cast<Window *>(glfwGetWindowUserPointer(handle));

                                   if (!window)
                                   {
                                       return;
                                   }

                                   WindowClosedEvent event;
                                   window->raise_event(event);
                               });

    glfwSetWindowFocusCallback(m_handle,
                               [](GLFWwindow *handle, int focused)
                               {
                                   const auto *window = static_cast<Window *>(glfwGetWindowUserPointer(handle));

                                   switch (focused)
                                   {
                                   case GLFW_TRUE:
                                   {
                                       WindowFocussedEvent event;
                                       window->raise_event(event);
                                       break;
                                   }
                                   case GLFW_FALSE:
                                   {
                                       WindowUnfocussedEvent event;
                                       window->raise_event(event);
                                       break;
                                   }
                                   default:;
                                   }
                               });

    glfwSetWindowIconifyCallback(m_handle,
                                 [](GLFWwindow *handle, int iconified)
                                 {
                                     const auto *window = static_cast<Window *>(glfwGetWindowUserPointer(handle));

                                     if (iconified == GLFW_TRUE)
                                     {
                                         WindowMinimizedEvent event;
                                         window->raise_event(event);
                                     }
                                 });

    glfwSetWindowMaximizeCallback(m_handle,
                                  [](GLFWwindow *handle, int maximize)
                                  {
                                      const auto *window = static_cast<Window *>(glfwGetWindowUserPointer(handle));

                                      if (maximize == GLFW_TRUE)
                                      {
                                          WindowMaximizedEvent event;
                                          window->raise_event(event);
                                      }
                                  });

    glfwSetWindowSizeCallback(m_handle,
                              [](GLFWwindow *handle, int width, int height)
                              {
                                  const auto *window = static_cast<Window *>(glfwGetWindowUserPointer(handle));

                                  WindowResizedEvent event(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
                                  window->raise_event(event);
                              });

    glfwSetWindowPosCallback(m_handle,
                             [](GLFWwindow *handle, int x, int y)
                             {
                                 const auto *window = static_cast<Window *>(glfwGetWindowUserPointer(handle));

                                 WindowMovedEvent event(x, y);
                                 window->raise_event(event);
                             });

    glfwSetKeyCallback(m_handle,
                       [](GLFWwindow *handle, int key, int scancode, int action, int mods)
                       {
                           ImGui_ImplGlfw_KeyCallback(handle, key, scancode, action, mods);

                           const auto *window = static_cast<Window *>(glfwGetWindowUserPointer(handle));

                           switch (action)
                           {
                           case GLFW_PRESS:
                           case GLFW_REPEAT:
                           {
                               KeyPressedEvent event(key, action == GLFW_REPEAT);
                               window->raise_event(event);
                               break;
                           }
                           case GLFW_RELEASE:
                           {
                               KeyReleasedEvent event(key);
                               window->raise_event(event);
                               break;
                           }
                           default:;
                           }
                       });

    glfwSetMouseButtonCallback(m_handle,
                               [](GLFWwindow *handle, int button, int action, int mods)
                               {
                                   ImGui_ImplGlfw_MouseButtonCallback(handle, button, action, mods);

                                   const auto *window = static_cast<Window *>(glfwGetWindowUserPointer(handle));

                                   switch (action)
                                   {
                                   case GLFW_PRESS:
                                   {
                                       MouseButtonPressedEvent event(button);
                                       window->raise_event(event);
                                       break;
                                   }
                                   case GLFW_RELEASE:
                                   {
                                       MouseButtonReleasedEvent event(button);
                                       window->raise_event(event);
                                       break;
                                   }
                                   default:;
                                   }
                               });

    glfwSetScrollCallback(m_handle,
                          [](GLFWwindow *handle, double x_offset, double y_offset)
                          {
                              ImGui_ImplGlfw_ScrollCallback(handle, x_offset, y_offset);

                              const auto *window = static_cast<Window *>(glfwGetWindowUserPointer(handle));

                              MouseScrolledEvent event(x_offset, y_offset);
                              window->raise_event(event);
                          });

    glfwSetCursorPosCallback(m_handle,
                             [](GLFWwindow *handle, double x, double y)
                             {
                                 ImGui_ImplGlfw_CursorPosCallback(handle, x, y);

                                 const auto *window = static_cast<Window *>(glfwGetWindowUserPointer(handle));

                                 MouseMovedEvent event(x, y);
                                 window->raise_event(event);
                             });
}

auto Window::glfw_error_callback(int error, const char *description) -> void
{
    spdlog::error("GLFW error {}: {}", error, description);
}

} // namespace goxel::core

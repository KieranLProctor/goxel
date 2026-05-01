#include "application.h"

namespace core
{

static Application *s_application = nullptr;

Application::Application(ApplicationSpec spec) : m_spec(spec)
{
    m_window = std::make_shared<Window>(spec.window_spec);
    m_window->create();
}

Application::~Application()
{
    m_window->destroy();
    m_window = nullptr;

    glfwTerminate();
}

auto Application::run() -> void
{
    m_is_running = true;

    auto last_time = get_time();

    // Main Application loop
    while (m_is_running)
    {
        glfwPollEvents();

        if (m_window->should_close())
        {
            stop();
            break;
        }

        auto current_time = get_time();
        auto timestep = glm::clamp(current_time - last_time, 0.001f, 0.1f);
        last_time = current_time;

        m_window->update();
    }
}

auto Application::stop() -> void
{
    m_is_running = false;
}

auto Application::get_framebuffer_size() -> glm::vec2
{
    return m_window->get_framebuffer_size();
}

auto Application::get_window() -> std::shared_ptr<Window>
{
    return m_window;
}

auto Application::get_time() -> float
{
    return static_cast<float>(glfwGetTime());
}

auto Application::get() -> Application &
{
    assert(s_application);
    return *s_application;
}

} // namespace core

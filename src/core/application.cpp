#include "application.h"

#include "spdlog/spdlog.h"
#include <ranges>

namespace core
{

static Application *s_application = nullptr;

Application::Application(ApplicationSpec spec) : m_spec(spec)
{
    s_application = this;

    if (!glfwInit())
    {
        spdlog::error("Failed to initialise GLFW!");
        assert(false);
    }

    // Set window to app name if title is empty.
    if (m_spec.window_spec.title.empty())
    {
        m_spec.window_spec.title = m_spec.name;
    }

    m_spec.window_spec.event_callback = [this](Event &event) { raise_event(event); };

    m_window = std::make_shared<Window>(m_spec.window_spec);
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

        for (auto &layer : m_layer_stack)
        {
            layer->on_update(timestep);
        }

        for (auto &layer : m_layer_stack)
        {
            layer->on_render();
        }

        m_window->update();
    }
}

auto Application::stop() -> void
{
    m_is_running = false;
}

auto Application::raise_event(Event &event) -> void
{
    for (auto &layer : std::views::reverse(m_layer_stack))
    {
        layer->on_event(event);
        if (event.is_handled)
        {
            break;
        }
    }
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

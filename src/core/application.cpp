#include "application.h"

#include "imgui_context.h"
#include "spdlog/spdlog.h"
#include <ranges>
#include <utility>

namespace goxel::core
{

static Application *s_application = nullptr;

Application::Application(ApplicationSpec spec) : m_spec(std::move(spec))
{
    s_application = this;

    if (!glfwInit())
    {
        spdlog::critical("Failed to initialise GLFW!");
        assert(false);
    }

    if (m_spec.window_spec.title.empty())
    {
        m_spec.window_spec.title = m_spec.name;
    }

    m_spec.window_spec.event_callback = [this](Event &event) { raise_event(event); };

    m_window = std::make_shared<Window>(m_spec.window_spec);
    m_window->create();

    m_imgui.emplace(m_window->get_handle());
    m_imgui->apply_dark_theme();

    m_window->init_callbacks();
}

Application::~Application()
{
    m_layer_stack.clear();
    m_imgui.reset();

    m_window = nullptr;

    glfwTerminate();
    s_application = nullptr;
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
        auto timestep = static_cast<float>(glm::clamp(current_time - last_time, 0.001, 0.1));
        last_time = current_time;

        for (auto &layer : m_layer_stack)
        {
            layer->on_update(timestep);
        }

        m_imgui->begin_frame();

        for (auto &layer : m_layer_stack)
        {
            layer->on_render();
        }

        m_imgui->end_frame();

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
        if (event.handled())
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

auto Application::get_time() -> double
{
    return glfwGetTime();
}

auto Application::get() -> Application &
{
    assert(s_application);
    return *s_application;
}

} // namespace goxel::core

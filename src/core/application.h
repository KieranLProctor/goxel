#pragma once

#include "layer.h"
#include "window.h"

#include <memory>
#include <string>

namespace core
{

struct ApplicationSpec
{
    std::string name;
    WindowSpec window_spec;
};

class Application
{
  public:
    explicit Application(ApplicationSpec spec);
    ~Application();

    auto run() -> void;
    auto stop() -> void;

    auto raise_event(Event &event) -> void;

    template <typename TLayer, typename... Args>
        requires(std::is_base_of_v<Layer, TLayer>)
    auto push_layer(Args &&...args) -> void
    {
        m_layer_stack.push_back(std::make_unique<Layer>(std::forward<Args>(args)...));
    }

    template <typename TLayer>
        requires(std::is_base_of_v<Layer, TLayer>)
    auto push_layer() -> void
    {
        m_layer_stack.push_back(std::make_unique<TLayer>());
    }

    auto get_framebuffer_size() -> glm::vec2;
    auto get_window() -> std::shared_ptr<Window>;

    static auto get_time() -> float;
    static auto get() -> Application &;

  private:
    ApplicationSpec m_spec;
    std::shared_ptr<Window> m_window;
    std::vector<std::unique_ptr<Layer>> m_layer_stack;

    bool m_is_running = false;
};

} // namespace core

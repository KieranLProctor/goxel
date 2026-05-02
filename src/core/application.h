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
    Application(ApplicationSpec spec);
    ~Application();

    auto run() -> void;
    auto stop() -> void;

    auto raise_event(Event &event) -> void;

    auto get_framebuffer_size() -> glm::vec2;
    auto get_window() -> std::shared_ptr<Window>;

    // NOTE: Not sure if nicer way to be static so all methods
    // align while using auto.
    static auto get_time() -> float;
    static auto get() -> Application &;

  private:
    ApplicationSpec m_spec;
    std::shared_ptr<Window> m_window;
    bool m_is_running = false;

    std::vector<std::unique_ptr<Layer>> m_layer_stack;
};

} // namespace core

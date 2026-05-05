#include "main_layer.h"

#include "spdlog/spdlog.h"

#include "glad/glad.h"

#include <iostream>

namespace ui
{

MainLayer::MainLayer()
{
    spdlog::info("created MainLayer!");
}

auto MainLayer::on_update(float time_step) -> void {}

auto MainLayer::on_render() -> void
{
    // Render
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

auto MainLayer::on_event(core::Event &event) -> void
{
    std::cout << event.to_string() << std::endl;

    core::EventDispatcher dispatcher(event);
    dispatcher.dispatch<core::KeyPressedEvent>([this](core::KeyPressedEvent &e) { return on_keyboard_input(e); });
}

auto MainLayer::on_keyboard_input(core::KeyPressedEvent &event) -> bool
{
    return true;
}

} // namespace ui

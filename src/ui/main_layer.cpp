#include "main_layer.h"

#include "spdlog/spdlog.h"

#include <iostream>

namespace ui
{

MainLayer::MainLayer()
{
    spdlog::info("created MainLayer!");
}

auto MainLayer::on_update(float time_step) -> void {}

auto MainLayer::on_render() -> void {}

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

#pragma once

#include "../core/event.h"
#include "../core/layer.h"

namespace ui
{

class MainLayer : public core::Layer
{
  public:
    MainLayer();
    ~MainLayer() override;

    auto on_update(float time_step) -> void override;
    auto on_render() -> void override;
    auto on_event(core::Event &event) -> void override;
};

} // namespace ui

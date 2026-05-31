#pragma once

#include "event.h"

namespace goxel::core
{

class Layer
{
  public:
    virtual ~Layer() = default;

    virtual auto on_update(float time_step) -> void {};
    virtual auto on_render() -> void {};
    virtual auto on_event(Event &event) -> void {};
};

} // namespace core

#pragma once

namespace core
{

class Layer
{
  public:
    virtual ~Layer() = default;

    virtual auto on_event() -> void;
    virtual auto on_update(float time_step) -> void;
    virtual auto on_render() -> void;
};

} // namespace core

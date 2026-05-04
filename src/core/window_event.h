#pragma once

#include "event.h"

#include <format>

namespace core
{

class WindowClosedEvent : public Event
{
  public:
    WindowClosedEvent() {}

    EVENT_CLASS_TYPE(WINDOW_CLOSED)
};

class WindowResizedEvent : public Event
{
  public:
    WindowResizedEvent(uint32_t width, uint32_t height) : m_width(width), m_height(height) {}

    inline auto get_width() -> uint32_t
    {
        return m_width;
    }
    inline auto get_height() -> uint32_t
    {
        return m_height;
    }

    auto to_string() -> std::string override
    {
        return std::format("WindowResizeEvent: {}, {}", m_width, m_height);
    }

    EVENT_CLASS_TYPE(WINDOW_RESIZED);

  private:
    uint32_t m_width;
    uint32_t m_height;
};

} // namespace core

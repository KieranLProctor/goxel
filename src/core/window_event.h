#pragma once

#include "event.h"

#include <format>

namespace core
{

class WindowClosedEvent : public Event
{
  public:
    WindowClosedEvent() = default;

    EVENT_CLASS_TYPE(WINDOW_CLOSED)
};

class WindowFocussedEvent : public Event
{
  public:
    WindowFocussedEvent() = default;

    EVENT_CLASS_TYPE(WINDOW_FOCUSSED);
};

class WindowUnfocussedEvent : public Event
{
  public:
    WindowUnfocussedEvent() = default;

    EVENT_CLASS_TYPE(WINDOW_UNFOCUSSED);
};

class WindowMinimizedEvent : public Event
{
  public:
    WindowMinimizedEvent() = default;

    EVENT_CLASS_TYPE(WINDOW_MINIMIZED);
};

class WindowMaximizedEvent : public Event
{
  public:
    WindowMaximizedEvent() = default;

    EVENT_CLASS_TYPE(WINDOW_MAXIMIZED);
};

class WindowResizedEvent : public Event
{
  public:
    WindowResizedEvent(uint32_t width, uint32_t height) : m_width(width), m_height(height) {}

    auto get_width() -> uint32_t
    {
        return m_width;
    }
    auto get_height() -> uint32_t
    {
        return m_height;
    }

    auto to_string() const -> std::string override
    {
        return std::format("WindowResizeEvent: {}x{}", m_width, m_height);
    }

    EVENT_CLASS_TYPE(WINDOW_RESIZED);

  private:
    uint32_t m_width;
    uint32_t m_height;
};

class WindowMovedEvent : public Event
{
  public:
    WindowMovedEvent(int x, int y) : m_x(x), m_y(y) {}

    auto get_x() const -> int
    {
        return m_x;
    }
    auto get_y() const -> int
    {
        return m_y;
    }

    auto to_string() const -> std::string override
    {
        return std::format("WindowMovedEvent: {}, {}", m_x, m_y);
    }

    EVENT_CLASS_TYPE(WINDOW_MOVED);

  private:
    int m_x;
    int m_y;
};

} // namespace core

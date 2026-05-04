#pragma once

#include "event.h"

#include <format>

namespace core
{

class KeyEvent : public Event
{
  public:
    auto get_key_code() -> int
    {
        return m_key_code;
    };

  protected:
    KeyEvent(int key_code) : m_key_code(key_code) {}
    int m_key_code;
};

class KeyPressedEvent : public KeyEvent
{
  public:
    KeyPressedEvent(int key_code, bool is_repeat) : KeyEvent(key_code), m_is_repeat(is_repeat) {}

    auto is_repeat() -> bool
    {
        return m_is_repeat;
    }

    auto to_string() -> std::string override
    {
        return std::format("KeyPressedEvent: {} (is_repeat={})", m_key_code, m_is_repeat);
    }

    EVENT_CLASS_TYPE(KEY_PRESSED);

  private:
    bool m_is_repeat;
};

class KeyReleasedEvent : public KeyEvent
{
  public:
    KeyReleasedEvent(int key_code) : KeyEvent(key_code) {}

    auto to_string() -> std::string override
    {
        return std::format("KeyReleasedEvent: {}", m_key_code);
    }

    EVENT_CLASS_TYPE(KEY_RELEASED)
};

class MouseMovedEvent : public Event
{
  public:
    MouseMovedEvent(int x, int y) : m_mouse_x(x), m_mouse_y(y) {}

    inline auto get_x() const -> double
    {
        return m_mouse_x;
    }
    inline auto get_y() const -> double
    {
        return m_mouse_y;
    }

    auto to_string() -> std::string override
    {
        return std::format("MouseMovedEvent: {}, {}", m_mouse_x, m_mouse_y);
    }

    EVENT_CLASS_TYPE(MOUSE_MOVED);

  private:
    double m_mouse_x;
    double m_mouse_y;
};

class MouseScrolledEvent : public Event
{
  public:
    MouseScrolledEvent(double x_offset, double y_offset) : m_x_offset(x_offset), m_y_offset(y_offset) {}

    inline auto get_x_offset() const -> double
    {
        return m_x_offset;
    }
    inline auto get_y_offset() const -> double
    {
        return m_y_offset;
    }

    auto to_string() -> std::string override
    {
        return std::format("MouseScrolledEvent: {}, {}", m_x_offset, m_y_offset);
    }

    EVENT_CLASS_TYPE(MOUSE_SCROLLED);

  private:
    double m_x_offset;
    double m_y_offset;
};

class MouseButtonEvent : public Event
{
  public:
    inline auto get_mouse_button() const -> int
    {
        return m_button;
    }

  protected:
    MouseButtonEvent(int button) : m_button(button) {}

    int m_button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
  public:
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

    auto to_string() -> std::string override
    {
        return std::format("MouseButtonPressedEvent: {}", m_button);
    }

    EVENT_CLASS_TYPE(MOUSE_BUTTON_PRESSED)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
  public:
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

    auto to_string() -> std::string override
    {
        return std::format("MouseButtonReleasedEvent: {}", m_button);
    }

    EVENT_CLASS_TYPE(MOUSE_BUTTON_RELEASED);
};

} // namespace core

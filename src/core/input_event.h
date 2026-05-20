#pragma once

#include "event.h"

#include <format>

namespace core
{

class KeyEvent : public Event
{
  public:
    auto get_key_code() const -> int
    {
        return m_key_code;
    };

  protected:
    explicit KeyEvent(int key_code) : m_key_code(key_code) {}

  private:
    int m_key_code;
};

class KeyPressedEvent : public KeyEvent
{
  public:
    KeyPressedEvent(int key_code, bool is_repeat) : KeyEvent(key_code), m_is_repeat(is_repeat) {}

    auto is_repeat() const -> bool
    {
        return m_is_repeat;
    }

    auto to_string() const -> std::string override
    {
        return std::format("KeyPressedEvent: {} (is_repeat={})", get_key_code(), m_is_repeat);
    }

    EVENT_CLASS_TYPE(KEY_PRESSED);

  private:
    bool m_is_repeat;
};

class KeyReleasedEvent : public KeyEvent
{
  public:
    explicit KeyReleasedEvent(int key_code) : KeyEvent(key_code) {}

    auto to_string() const -> std::string override
    {
        return std::format("KeyReleasedEvent: {}", get_key_code());
    }

    EVENT_CLASS_TYPE(KEY_RELEASED)
};

class MouseMovedEvent : public Event
{
  public:
    MouseMovedEvent(double x, double y) : m_mouse_x(x), m_mouse_y(y)
    {
        m_mouse_dx = x - s_last_x;
        m_mouse_dy = y - s_last_y;
        s_last_x = x;
        s_last_y = y;
    }

    auto get_dx() const -> double
    {
        return m_mouse_dx;
    }

    auto get_dy() const -> double
    {
        return m_mouse_dy;
    }

    auto get_x() const -> double
    {
        return m_mouse_x;
    }
    auto get_y() const -> double
    {
        return m_mouse_y;
    }

    auto to_string() const -> std::string override
    {
        return std::format("MouseMovedEvent: {}, {} (delta: {}, {})", m_mouse_x, m_mouse_y, m_mouse_dx, m_mouse_dy);
    }

    EVENT_CLASS_TYPE(MOUSE_MOVED);

  private:
    double m_mouse_dx;
    double m_mouse_dy;
    double m_mouse_x;
    double m_mouse_y;

    inline static double s_last_x = 0.0;
    inline static double s_last_y = 0.0;
};

class MouseScrolledEvent : public Event
{
  public:
    MouseScrolledEvent(double x_offset, double y_offset) : m_x_offset(x_offset), m_y_offset(y_offset) {}

    auto get_x_offset() const -> double
    {
        return m_x_offset;
    }
    auto get_y_offset() const -> double
    {
        return m_y_offset;
    }

    auto to_string() const -> std::string override
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
    auto get_mouse_button() const -> int
    {
        return m_button;
    }

  protected:
    explicit MouseButtonEvent(int button) : m_button(button) {}

  private:
    int m_button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
  public:
    explicit MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

    auto to_string() const -> std::string override
    {
        return std::format("MouseButtonPressedEvent: {}", get_mouse_button());
    }

    EVENT_CLASS_TYPE(MOUSE_BUTTON_PRESSED)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
  public:
    explicit MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

    auto to_string() const -> std::string override
    {
        return std::format("MouseButtonReleasedEvent: {}", get_mouse_button());
    }

    EVENT_CLASS_TYPE(MOUSE_BUTTON_RELEASED);
};

} // namespace core

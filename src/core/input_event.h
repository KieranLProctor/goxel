#pragma once

#include "event.h"

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
    EVENT_CLASS_TYPE(KEY_PRESSED);
};

class KeyReleasedEvent : public KeyEvent
{
  public:
    KeyReleasedEvent(int key_code) : KeyEvent(key_code) {}

    EVENT_CLASS_TYPE(KEY_RELEASED)
};

} // namespace core

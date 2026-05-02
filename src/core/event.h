#pragma once

#include <string>

namespace core
{

enum class EventType
{
    NONE = 0,
    WINDOW_CLOSE,
    WINDOW_RESIZE,
    WINDOW_MOVE,
    KEY_PRESSED,
    KEY_RELEASED,
    MOUSE_BUTTON_PRESSED,
    MOUSE_BUTTON_RELEASED,
    MOUSE_MOVED,
    MOUSE_SCROLLED,
};

#define EVENT_CLASS_TYPE(type)                                                                                         \
    static auto get_static_type() -> EventType                                                                         \
    {                                                                                                                  \
        return EventType::type;                                                                                        \
    }                                                                                                                  \
    virtual auto get_event_type() -> EventType const override                                                          \
    {                                                                                                                  \
        return get_static_type();                                                                                      \
    }                                                                                                                  \
    virtual auto get_name() -> const char *override                                                                    \
    {                                                                                                                  \
        return #type;                                                                                                  \
    }

class Event
{
  public:
    bool is_handled = false;

    virtual ~Event() {};
    virtual auto get_event_type() -> EventType const = 0;
    virtual auto get_name() -> const char * = 0;
    virtual auto to_string() -> std::string
    {
        return get_name();
    }
};

class EventDispatcher
{
  public:
    EventDispatcher(Event &event) : m_event(event) {}

    template <typename T, typename F> bool dispatch(F &&func)
    {
        if (m_event.get_event_type() == T::get_static_type() && !m_event.is_handled)
        {
            m_event.is_handled = (func)(static_cast<T &>(m_event));

            return true;
        }

        return false;
    }

  private:
    Event &m_event;
};

} // namespace core

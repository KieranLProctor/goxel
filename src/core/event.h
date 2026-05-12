#pragma once

#include <string>
#include <string_view>

namespace core
{

enum class EventType
{
    NONE = 0,
    WINDOW_CLOSED,
    WINDOW_RESIZED,
    WINDOW_MOVED,
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
    virtual auto get_event_type() const -> EventType override                                                          \
    {                                                                                                                  \
        return get_static_type();                                                                                      \
    }                                                                                                                  \
    virtual auto get_name() const -> std::string_view override                                                         \
    {                                                                                                                  \
        return #type;                                                                                                  \
    }

class Event
{
  public:
    bool is_handled = false;

    virtual ~Event() = default;

    virtual auto get_event_type() const -> EventType = 0;
    virtual auto get_name() const -> std::string_view = 0;
    virtual auto to_string() const -> std::string
    {
        return std::string(get_name());
    }

    auto handled() const -> bool
    {
        return is_handled;
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
            m_event.is_handled = std::forward<F>(func)(static_cast<T &>(m_event));

            return true;
        }

        return false;
    }

  private:
    Event &m_event;
};

} // namespace core

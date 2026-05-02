#pragma once

#include "event.h"

namespace core
{

class WindowClosedEvent : public Event
{
public:
    WindowClosedEvent() {}

    EVENT_CLASS_TYPE(WINDOW_CLOSE)
};

}
#pragma once

#include <entt/entt.hpp>

namespace game
{

inline entt::registry &get_registry()
{
    static entt::registry registry{};

    return registry;
}

}
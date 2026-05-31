#pragma once

#include <entt/entt.hpp>

namespace goxel::game
{

inline entt::registry &get_registry()
{
    static entt::registry registry{};

    return registry;
}

}
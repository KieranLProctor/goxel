#pragma once

#include "glm/gtc/quaternion.hpp"
#include "glm/glm.hpp"

namespace goxel::game::components
{

struct Transform
{
    glm::vec3 position{0.0f};
    glm::quat rotation{};
    glm::vec3 scale{1.0f};
};

} // namespace game::components

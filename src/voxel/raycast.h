#pragma once

#include "chunk.h"

#include <optional>

namespace goxel::voxel
{

struct RaycastHit
{
    glm::ivec3 block;
    glm::ivec3 placement;
};

auto raycast(const Chunk &chunk, glm::vec3 origin, glm::vec3 direction, float max_distance) -> std::optional<RaycastHit>;

} // namespace goxel::voxel
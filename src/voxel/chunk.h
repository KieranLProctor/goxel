#pragma once

#include "block.h"
#include "glm/glm.hpp"

#include <array>

namespace goxel::voxel
{

// Size of the chunk in blocks x*x*x.
static constexpr int S_CHUNK_SIZE = 16;

struct Chunk
{
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    std::array<Block, S_CHUNK_SIZE * S_CHUNK_SIZE * S_CHUNK_SIZE> blocks{};

    auto get_block(glm::vec3 position) const -> Block;
    auto set_block(glm::vec3 position, Block block) -> void;

    static auto in_bounds(glm::vec3 position) -> bool;

  private:
    static auto to_index(glm::vec3 position) -> int;
};

} // namespace goxel::voxel

#pragma once

#include "glm/vec3.hpp"

#include <array>
#include <cstdint>

namespace goxel::voxel
{

enum class Block : uint16_t
{
    AIR = 0,
    DIRT = 1,
    GRASS = 2,
    WOOD = 3,
    LEAVES = 4,
    STONE = 5,
    SAND = 6,
    WATER = 7,
    COUNT
};

struct BlockProperties
{
    bool is_solid = true;
    bool is_transparent = false;
    glm::vec3 colour = {1.0f, 0.0f, 1.0f};
    const char *name = "UNKNOWN";
};

inline constexpr std::array<BlockProperties, static_cast<size_t>(Block::COUNT)> k_block_table = {
    {
     /* AIR    */ {.is_solid = false, .is_transparent = true, .name = "AIR"},
     /* DIRT   */ {.colour = {0.6f, 0.4f, 0.2f}, .name = "DIRT"},
     /* GRASS  */ {.colour = {0.0f, 0.8f, 0.2f}, .name = "GRASS"},
     /* WOOD   */ {.colour = {0.7f, 0.5f, 0.2f}, .name = "WOOD"},
     /* LEAVES */ {.is_solid = true, .is_transparent = true, .colour = {0.0f, 0.7f, 0.1f}, .name = "LEAVES"},
     /* STONE  */ {.colour = {0.5f, 0.5f, 0.5f}, .name = "STONE"},
     /* SAND   */ {.name = "SAND"},
     /* WATER  */ {.is_solid = false, .is_transparent = true, .colour = {0.1f, 0.3f, 0.9f}, .name = "WATER"},
     }
};

inline auto get_block_properties(const Block block) -> const BlockProperties &
{
    return k_block_table[static_cast<size_t>(block)];
}

} // namespace goxel::voxel

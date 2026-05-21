#pragma once

#include <cstdint>

namespace game
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
};

inline auto get_block_properties(Block block) -> BlockProperties
{
    switch (block)
    {
    case Block::AIR: return {false, true};
    case Block::LEAVES: return {true, true};
    case Block::WATER: return {false, true};
    default: return {true, false};
    }
}

} // namespace game

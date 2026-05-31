#pragma once

#include "block.h"

namespace game::components
{

struct Renderable
{
    Block block_type = Block::STONE;

    explicit Renderable(Block type = Block::STONE) : block_type(type) {}
};

} // namespace game::components
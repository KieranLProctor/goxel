#pragma once

#include "block.h"

namespace goxel::game::components
{

struct Renderable
{
    voxel::Block block_type = voxel::Block::STONE;

    explicit Renderable(const voxel::Block type = voxel::Block::STONE) : block_type(type) {}
};

} // namespace game::components
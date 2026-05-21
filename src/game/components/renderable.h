#pragma once

namespace game::components
{

struct Renderable
{
    Block block_type = Block::STONE;

    explicit Renderable(Block type = Block::STONE) : block_type(type) {}
};

}
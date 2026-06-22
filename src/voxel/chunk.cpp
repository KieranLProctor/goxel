#include "chunk.h"

namespace goxel::voxel
{

auto Chunk::to_index(const glm::vec3 position) -> int
{
    return static_cast<int>(position.x + S_CHUNK_SIZE * (position.y + S_CHUNK_SIZE * position.z));
}

auto Chunk::in_bounds(const glm::vec3 position) -> bool
{
    return position.x >= 0 && position.x < S_CHUNK_SIZE && position.y >= 0 && position.y < S_CHUNK_SIZE &&
           position.z >= 0 && position.z < S_CHUNK_SIZE;
}

auto Chunk::get_block(const glm::vec3 position) const -> Block
{
    if (!in_bounds(position))
    {
        return Block::AIR;
    }

    return blocks[to_index(position)];
}

auto Chunk::set_block(const glm::vec3 position, const Block block) -> void
{
    if (!in_bounds(position))
    {
        return;
    }

    blocks[to_index(position)] = block;
}

} // namespace goxel::voxel

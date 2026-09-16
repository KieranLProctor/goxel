#pragma once

#include "chunk.h"

#include <vector>

namespace goxel::voxel
{

auto build_chunk_mesh(const Chunk &chunk) -> std::vector<float>;

}
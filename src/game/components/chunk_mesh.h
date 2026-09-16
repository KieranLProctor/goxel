#pragma once

#include "handles.h"

namespace goxel::game::components
{

struct ChunkMesh
{
    rendering::MeshHandle handle = rendering::MeshHandle::INVALID;

    explicit ChunkMesh(const rendering::MeshHandle mesh_handle = rendering::MeshHandle::INVALID) : handle(mesh_handle)
    {
    }
};

} // namespace goxel::game::components

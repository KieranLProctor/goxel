#pragma once

#include "handles.h"
#include "mesh.h"

#include <unordered_map>

namespace goxel::rendering
{

class MeshRegistry
{
public:
    ~MeshRegistry();

    auto create_mesh(const float *vertices, int vertex_count) -> MeshHandle;
    auto destroy_mesh(MeshHandle handle) -> void;

    auto get(MeshHandle handle) -> const Mesh*;

private:
    std::unordered_map<GLuint, Mesh> m_meshes;
    GLuint m_next_id = 1;
};

} // namespace goxel::rendering
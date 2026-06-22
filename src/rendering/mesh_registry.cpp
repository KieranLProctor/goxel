#include "mesh_registry.h"

#include <ranges>

namespace goxel::rendering
{

MeshRegistry::~MeshRegistry()
{
    for (auto &mesh : m_meshes | std::views::values)
    {
        glDeleteVertexArrays(1, &mesh.vao);
        glDeleteBuffers(1, &mesh.vbo);
    }
}

auto MeshRegistry::create_mesh(const float *vertices, const int vertex_count) -> MeshHandle
{
    Mesh mesh;
    mesh.vertex_count = vertex_count;
}

} // namespace goxel::rendering

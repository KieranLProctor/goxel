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
    constexpr GLsizei k_floats_per_vertex = 6; // position (3) + colour (3)

    Mesh mesh;
    mesh.vertex_count = vertex_count;

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertex_count) * k_floats_per_vertex * sizeof(float), vertices,
                 GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, k_floats_per_vertex * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    // colour
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, k_floats_per_vertex * sizeof(float),
                          reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    const GLuint id = m_next_id++;
    m_meshes[id] = mesh;

    return static_cast<MeshHandle>(id);
}

auto MeshRegistry::destroy_mesh(const MeshHandle handle) -> void
{
    const auto it = m_meshes.find(static_cast<GLuint>(handle));

    if (it == m_meshes.end())
    {
        return;
    }

    glDeleteVertexArrays(1, &it->second.vao);
    glDeleteBuffers(1, &it->second.vbo);

    m_meshes.erase(it);
}

auto MeshRegistry::get(const MeshHandle handle) const -> const Mesh *
{
    const auto it = m_meshes.find(static_cast<GLuint>(handle));

    return it == m_meshes.end() ? nullptr : &it->second;
}

} // namespace goxel::rendering

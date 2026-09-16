#include "render_system.h"

#include "components/chunk_mesh.h"
#include "components/renderable.h"
#include "components/transform.h"
#include "ecs.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace goxel::game::systems
{

// RenderSystem::RenderSystem(rendering::Camera &camera) : m_camera(camera) {}

RenderSystem::~RenderSystem()
{
    glDeleteProgram(m_shader);
}

auto RenderSystem::init() -> void
{
    if (m_shader == 0)
    {
        return;
    }

    m_loc_model = glGetUniformLocation(m_shader, "uModel");
    m_loc_view = glGetUniformLocation(m_shader, "uView");
    m_loc_projection = glGetUniformLocation(m_shader, "uProjection");
}

auto RenderSystem::update() const -> void
{
    if (m_shader == 0)
    {
        return;
    }

    glUseProgram(m_shader);

    auto &registry = get_registry();
    const auto &camera = registry.ctx().get<rendering::Camera>();

    glm::mat4 view = camera.get_view_matrix();
    glm::mat4 projection = camera.get_projection_matrix();

    glUniformMatrix4fv(m_loc_view, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(m_loc_projection, 1, GL_FALSE, &projection[0][0]);

    auto render_view = registry.view<components::Transform, components::ChunkMesh>();

    for (const auto entity : render_view)
    {
        auto &transform = render_view.get<components::Transform>(entity);
        auto &chunk_mesh = render_view.get<components::ChunkMesh>(entity);

        const auto *mesh = m_mesh_registry.get(chunk_mesh.handle);

        if (mesh == nullptr)
        {
            continue;
        }

        // glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position) * glm::mat4_cast(transform.rotation) *
                          glm::scale(glm::mat4(1.0f), transform.scale);

        glUniformMatrix4fv(m_loc_model, 1, GL_FALSE, &model[0][0]);

        glBindVertexArray(mesh->vao);
        glDrawArrays(GL_TRIANGLES, 0, mesh->vertex_count);
    }

    glBindVertexArray(0);
    glUseProgram(0);
}

auto RenderSystem::set_shader(const GLuint shader) -> void
{
    m_shader = shader;
    m_loc_model = glGetUniformLocation(m_shader, "uModel");
    m_loc_view = glGetUniformLocation(m_shader, "uView");
    m_loc_projection = glGetUniformLocation(m_shader, "uProjection");
}

auto RenderSystem::get_mesh_registry() -> rendering::MeshRegistry &
{
    return m_mesh_registry;
}

} // namespace goxel::game::systems

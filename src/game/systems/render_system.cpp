#include "render_system.h"

#include "components/renderable.h"
#include "components/transform.h"
#include "ecs.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"

namespace goxel::game::systems
{

RenderSystem::RenderSystem(rendering::Camera &camera) : m_camera(camera) {}

RenderSystem::~RenderSystem()
{
    if (m_shader != 0)
    {
        glDeleteProgram(m_shader);
    }

    if (m_vao != 0)
    {
        glDeleteVertexArrays(1, &m_vao);
    }

    if (m_vbo != 0)
    {
        glDeleteBuffers(1, &m_vbo);
    }
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

auto RenderSystem::update() -> void
{
    if (m_shader == 0 || m_vao == 0)
    {
        return;
    }

    glUseProgram(m_shader);
    glBindVertexArray(m_vao);

    glm::mat4 view = m_camera.get_view_matrix();
    glm::mat4 projection = m_camera.get_projection_matrix();

    glUniformMatrix4fv(m_loc_view, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(m_loc_projection, 1, GL_FALSE, &projection[0][0]);

    auto &registry = get_registry();
    auto render_view = registry.view<components::Transform, components::Renderable>();

    for (auto entity : render_view)
    {
        auto &transform = render_view.get<components::Transform>(entity);
        auto &renderable = render_view.get<components::Renderable>(entity);

        glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position);
        glm::vec3 colour = get_block_colour(renderable.block_type);

        glUniformMatrix4fv(m_loc_model, 1, GL_FALSE, &model[0][0]);
        glUniform3fv(glGetUniformLocation(m_shader, "uBlockColour"), 1, &colour[0]);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindVertexArray(0);
    glUseProgram(0);
}

auto RenderSystem::set_shader(GLuint shader) -> void
{
    m_shader = shader;
    m_loc_model = glGetUniformLocation(m_shader, "uModel");
    m_loc_view = glGetUniformLocation(m_shader, "uView");
    m_loc_projection = glGetUniformLocation(m_shader, "uProjection");
}

auto RenderSystem::set_vao(GLuint vao) -> void
{
    m_vao = vao;
}

auto RenderSystem::set_vbo(GLuint vbo) -> void
{
    m_vbo = vbo;
}

auto RenderSystem::get_block_colour(voxel::Block block) -> glm::vec3
{
    switch (block)
    {
    case voxel::Block::GRASS: return {0.0f, 0.8f, 0.2f};
    case voxel::Block::DIRT: return {0.6f, 0.4f, 0.2f};
    case voxel::Block::STONE: return {0.5f, 0.5f, 0.5f};
    case voxel::Block::WOOD: return {0.7f, 0.5f, 0.2f};
    case voxel::Block::LEAVES: return {0.0f, 0.7f, 0.1f};
    default: return {1.0f, 0.0f, 1.0f};
    }
}

} // namespace game::systems

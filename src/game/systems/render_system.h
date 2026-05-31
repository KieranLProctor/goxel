#pragma once

#include "block.h"
#include "camera.h"
#include "glad/glad.h"

namespace goxel::game::systems
{

class RenderSystem
{
  public:
    explicit RenderSystem(rendering::Camera &camera);
    ~RenderSystem();

    auto init() -> void;
    auto update() -> void;

    auto set_shader(GLuint shader) -> void;
    auto set_vao(GLuint vao) -> void;
    auto set_vbo(GLuint vbo) -> void;

    auto get_block_colour(voxel::Block block) -> glm::vec3;

  private:
    rendering::Camera &m_camera;
    GLuint m_shader = 0;
    GLuint m_vao = 0;
    GLuint m_vbo = 0;

    GLint m_loc_model = -1;
    GLint m_loc_view = -1;
    GLint m_loc_projection = -1;
};

} // namespace game::systems

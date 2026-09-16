#pragma once

#include "block.h"
#include "camera.h"
#include "glad/glad.h"
#include "mesh_registry.h"

namespace goxel::game::systems
{

class RenderSystem
{
  public:
    RenderSystem() = default;
    ~RenderSystem();

    auto init() -> void;
    auto update() const -> void;

    auto set_shader(GLuint shader) -> void;

    auto get_mesh_registry() -> rendering::MeshRegistry &;

  private:
    GLuint m_shader = 0;

    GLint m_loc_model = -1;
    GLint m_loc_view = -1;
    GLint m_loc_projection = -1;

    rendering::MeshRegistry m_mesh_registry;
};

} // namespace game::systems

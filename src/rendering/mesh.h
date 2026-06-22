#pragma once

#include "glad/glad.h"

namespace goxel::rendering
{

struct Mesh
{
    GLuint vao = 0;
    GLuint vbo = 0;
    int vertex_count = 0;
};

} // namespace goxel::rendering

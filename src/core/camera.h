#pragma once

#include "glm/glm.hpp"

namespace core
{

class Camera
{
public:
    Camera();
    ~Camera();

    auto get_projection() -> glm::mat4;

private:
    glm::mat4 m_projection;
};

}
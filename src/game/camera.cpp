#include "camera.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace game
{

Camera::Camera(const float yaw, const float pitch) : m_yaw(yaw), m_pitch(pitch)
{
    update_vectors();
}

auto Camera::get_mode() const -> CameraMode
{
    return m_mode;
}

auto Camera::set_mode(CameraMode mode) -> void
{
    m_mode = mode;
}

auto Camera::cycle_mode() -> void
{
    switch (m_mode)
    {
    case CameraMode::FIRST_PERSON:
    {
        m_mode = CameraMode::FIRST_PERSON;
        break;
    }
    case CameraMode::THIRD_PERSON:
    {
        m_mode = CameraMode::THIRD_PERSON;
        break;
    }
    case CameraMode::THIRD_PERSON_SELF:
    {
        m_mode = CameraMode::THIRD_PERSON_SELF;
        break;
    }
    }
}

auto Camera::get_position() -> glm::vec3
{
    return m_position;
}

auto Camera::set_position(glm::vec3 position) -> void
{
    m_position = position;
}

auto Camera::get_yaw() -> float
{
    return m_yaw;
}

auto Camera::get_pitch() -> float
{
    return m_pitch;
}

auto Camera::get_look() const -> glm::vec3
{
    return m_look;
}

auto Camera::get_front() const -> glm::vec3
{
    return m_front;
}

auto Camera::get_up() const -> glm::vec3
{
    return m_up;
}

auto Camera::get_right() const -> glm::vec3
{
    return m_right;
}

auto Camera::adjust_pitch(float delta) -> void
{
    m_pitch = glm::clamp(m_pitch + delta, -k_pitch_limit, k_pitch_limit);
    update_vectors();
}

auto Camera::adjust_yaw(float delta) -> void
{
    m_yaw += delta;
    if (m_yaw > 180.0f)
    {
        m_yaw -= 360.0f;
    }

    if (m_yaw < -180.0f)
    {
        m_yaw += 360.0f;
    }

    update_vectors();
}

auto Camera::set_viewport(int width, int height) -> void
{
    m_width = width;
    m_height = height;
}

auto Camera::get_fov() const -> float
{
    return m_fov;
}

auto Camera::set_fov(float fov) -> void
{
    m_fov = glm::clamp(fov, 1.0f, 200.0f);
}

auto Camera::get_view_matrix() const -> glm::mat4
{
    auto view = compute_view();

    if (m_mode == CameraMode::THIRD_PERSON_SELF)
    {
        // Look back at self.
        return glm::lookAt(view, m_position, m_world_up);
    }

    return glm::lookAt(view, view + m_look, m_up);
}

auto Camera::get_projection_matrix() const -> glm::mat4
{
    return glm::perspective(glm::radians(m_fov), static_cast<float>(m_width) / static_cast<float>(m_height), m_near_plane, m_far_plane);
}

auto Camera::get_view_projection() const -> glm::mat4
{
    return get_projection_matrix() * get_view_matrix();
}

auto Camera::update_vectors() -> void
{
    glm::vec3 look;
    look.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    look.y = sin(glm::radians(m_pitch));
    look.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_look = glm::normalize(look);

    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw));
    front.y = 0.0f;
    front.z = sin(glm::radians(m_yaw));
    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(m_look, m_world_up));
    m_up = glm::normalize(glm::cross(m_right, m_look));
}

auto Camera::compute_view() const -> glm::vec3
{
    return m_position;
}

} // namespace game

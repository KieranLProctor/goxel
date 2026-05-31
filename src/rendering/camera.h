#pragma once

#include "glm/glm.hpp"

namespace goxel::rendering
{

enum class CameraMode
{
    FIRST_PERSON,
    THIRD_PERSON,
    THIRD_PERSON_SELF
};

class Camera
{
  public:
    explicit Camera(float yaw, float pitch);
    ~Camera() = default;

    auto get_mode() const -> CameraMode;
    auto set_mode(CameraMode mode) -> void;
    auto cycle_mode() -> void;

    auto get_position() const -> glm::vec3;
    auto set_position(glm::vec3 position) -> void;

    auto get_yaw() const -> float;
    auto get_pitch() const -> float;

    auto get_look() const -> glm::vec3;
    auto get_front() const -> glm::vec3;
    auto get_up() const -> glm::vec3;
    auto get_right() const -> glm::vec3;

    auto adjust_pitch(float delta) -> void;
    auto adjust_yaw(float delta) -> void;
    auto translate(glm::vec3 delta) -> void;

    auto set_viewport(int width, int height) -> void;

    auto get_fov() const -> float;
    auto set_fov(float fov) -> void;
    auto get_near_plane() const -> float;
    auto get_far_plane() const -> float;
    auto get_zoom() const -> float;
    auto set_zoom(float zoom) -> void;

    auto get_view_matrix() const -> glm::mat4;
    auto get_projection_matrix() const -> glm::mat4;
    auto get_view_projection() const -> glm::mat4;

  private:
    glm::vec3 m_position = {0.0f, 2.0f, 10.0f};

    float m_yaw = 0.0f;
    float m_pitch = 0.0f;

    glm::vec3 m_look = {0.0f, 1.0f, 0.0f};
    glm::vec3 m_front = {0.0f, 0.0f, -1.0f};
    glm::vec3 m_up = {0.0f, 1.0f, 0.0f};
    glm::vec3 m_right = {1.0f, 0.0f, 0.0f};
    glm::vec3 m_world_up = {0.0f, 1.0f, 0.0f};

    CameraMode m_mode = CameraMode::FIRST_PERSON;
    float m_fov = 90.0f;
    float m_near_plane = 0.1f;
    float m_far_plane = 2000.0f;
    float m_zoom = 1.0f;
    int m_width = 1280;
    int m_height = 720;

    static constexpr float k_pitch_limit = 90.0f;
    static constexpr float k_third_person_offset = 5.0f;

    auto update_vectors() -> void;
    auto compute_view() const -> glm::vec3;
};

} // namespace rendering

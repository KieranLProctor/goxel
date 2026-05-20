#include "main_layer.h"

#include "../game/camera.h"
#include "application.h"
#include "glad/glad.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "imgui.h"
#include "renderer/shader.h"
#include "spdlog/spdlog.h"

namespace ui
{

MainLayer::MainLayer()
{
    spdlog::info("created MainLayer!");

    float vertices[] = {
        // left face (red)
        -1.0f,-1.0f,-1.0f,  1.0f, 0.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,  1.0f, 0.0f, 0.0f,
        -1.0f,-1.0f,-1.0f,  1.0f, 0.0f, 0.0f,

        // right face (green)
         1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,
         1.0f,-1.0f,-1.0f,  0.0f, 1.0f, 0.0f,
         1.0f,-1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
         1.0f,-1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
         1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,

        // bottom face (blue)
         1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,  0.0f, 0.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,  0.0f, 0.0f, 1.0f,
         1.0f,-1.0f,-1.0f,  0.0f, 0.0f, 1.0f,
         1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f,

        // top face (yellow)
         1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f,
         1.0f, 1.0f,-1.0f,  1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,  1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,  1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f,

        // back face (cyan)
         1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,  0.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 1.0f,
         1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 1.0f,
         1.0f,-1.0f,-1.0f,  0.0f, 1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,  0.0f, 1.0f, 1.0f,

        // front face (magenta)
        -1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,  1.0f, 0.0f, 1.0f,
         1.0f,-1.0f, 1.0f,  1.0f, 0.0f, 1.0f,
         1.0f,-1.0f, 1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f,
    };

    m_simple_shader =
        core::renderer::create_graphics_shader("assets/shaders/simple.vert", "assets/shaders/simple.frag");

    glGenVertexArrays(1, &m_simple_vao);
    glGenBuffers(1, &m_simple_vbo);
    glBindVertexArray(m_simple_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_simple_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    // colour
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    const auto frame_buffer = core::Application::get().get_framebuffer_size();
    m_camera.set_viewport(static_cast<int>(frame_buffer.x), static_cast<int>(frame_buffer.y));
}

MainLayer::~MainLayer()
{
    if (m_simple_shader != 0)
    {
        glDeleteBuffers(1, &m_simple_vbo);
        glDeleteVertexArrays(1, &m_simple_vao);
        glDeleteProgram(m_simple_shader);
    }
}

auto MainLayer::on_update(float time_step) -> void
{
    auto *window = core::Application::get().get_window()->get_handle();

    glm::vec3 move(0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        move += m_camera.get_front();
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        move -= m_camera.get_front();
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        move -= m_camera.get_right();
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        move += m_camera.get_right();
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        move += m_camera.get_up();
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        move -= m_camera.get_up();
    }

    if (glm::length(move) > 0.0f)
    {
        move = glm::normalize(move);
        m_camera.translate(move * m_move_speed * time_step);
    }
}

auto MainLayer::on_render() -> void
{
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_simple_shader);
    glBindVertexArray(m_simple_vao);

    glm::mat4 view = m_camera.get_view_matrix();
    glm::mat4 proj = m_camera.get_projection_matrix();
    glUniformMatrix4fv(glGetUniformLocation(m_simple_shader, "uView"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_simple_shader, "uProjection"), 1, GL_FALSE, &proj[0][0]);

    auto draw_cube = [&](glm::vec3 position)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        glUniformMatrix4fv(glGetUniformLocation(m_simple_shader, "uModel"), 1, GL_FALSE, &model[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    };

    draw_cube({ -3.0f, 0.0f, 0.0f});
    draw_cube({ 0.0f, 0.0f, 0.0f});
    draw_cube({3.0f, 0.0f, 0.0f});

    draw_cube({ -3.0f, -3.0f, 0.0f});
    draw_cube({ 0.0f, -3.0f, 0.0f});
    draw_cube({3.0f, -3.0f, 0.0f});

    glBindVertexArray(0);
    glUseProgram(0);

    // --- ImGui debug overlay ---
    ImGui::Begin("Camera Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    // Eye position
    auto eye = m_camera.get_position();
    ImGui::SeparatorText("Position");
    ImGui::Text("Eye:   %.2f, %.2f, %.2f", eye.x, eye.y, eye.z);

    // Direction vectors
    auto look = m_camera.get_look();
    auto front = m_camera.get_front();
    auto right = m_camera.get_right();
    auto up = m_camera.get_up();
    ImGui::SeparatorText("Vectors");
    ImGui::Text("Look:  %.2f, %.2f, %.2f", look.x, look.y, look.z);
    ImGui::Text("Front: %.2f, %.2f, %.2f", front.x, front.y, front.z);
    ImGui::Text("Right: %.2f, %.2f, %.2f", right.x, right.y, right.z);
    ImGui::Text("Up:    %.2f, %.2f, %.2f", up.x, up.y, up.z);

    // View stuff
    ImGui::SeparatorText("View");
    ImGui::Text("FOV: %.2f", m_camera.get_fov());
    ImGui::Text("Zoom: %.2f", m_camera.get_zoom());
    ImGui::Text("Near plane: %.2f", m_camera.get_near_plane());
    ImGui::Text("Far plane: %.2f", m_camera.get_far_plane());

    // Yaw / pitch - expose these via getters if you don't have them
    ImGui::SeparatorText("Orientation");
    ImGui::Text("Yaw:   %.2f", m_camera.get_yaw());
    ImGui::Text("Pitch: %.2f", m_camera.get_pitch());

    // Mouse deltas - add these members to main_layer.h
    ImGui::SeparatorText("Mouse");
    ImGui::Text("Sensitivity: %.2f", m_mouse_sensitivity);
    ImGui::SliderFloat("##sens", &m_mouse_sensitivity, 0.1f, 10.0f);

    // Triangle position for reference
    ImGui::SeparatorText("Object");
    ImGui::Text("Dist to eye: %.2f", glm::length(eye - glm::vec3(0.0f, 0.0f, -5.0f)));

    // Camera mode
    ImGui::SeparatorText("Mode");
    auto mode_str = "Unknown";
    switch (m_camera.get_mode())
    {
    case game::CameraMode::FIRST_PERSON: mode_str = "First Person"; break;
    case game::CameraMode::THIRD_PERSON: mode_str = "Third Person Back"; break;
    case game::CameraMode::THIRD_PERSON_SELF: mode_str = "Third Person Front"; break;
    }
    ImGui::Text("Mode: %s", mode_str);
    if (ImGui::Button("Cycle Mode (F5)"))
    {
        m_camera.cycle_mode();
    }
    ImGui::End();
}

auto MainLayer::on_event(core::Event &event) -> void
{
    core::EventDispatcher dispatcher(event);
    dispatcher.dispatch<core::KeyPressedEvent>([this](core::KeyPressedEvent &e) { return on_keyboard_input(e); });
    // dispatcher.dispatch<core::MouseButtonEvent>([this](core::MouseButtonEvent &e) { return on_mouse_input(e); });
    dispatcher.dispatch<core::MouseMovedEvent>([this](core::MouseMovedEvent &e) { return on_mouse_move(e); });
}

auto MainLayer::on_keyboard_input(core::KeyPressedEvent &event) -> bool
{
    if (event.get_key_code() == GLFW_KEY_F5)
    {
        m_camera.cycle_mode();
    }

    if (event.get_key_code() == GLFW_KEY_ESCAPE || event.get_key_code() == GLFW_KEY_E)
    {
        m_cursor_captured = !m_cursor_captured;
        core::Application::get().get_window()->set_cursor_captured(m_cursor_captured);
    }

    return true;
}

// auto MainLayer::on_mouse_input(core::MouseButtonEvent &event) -> bool
// {
//     return true;
// }

auto MainLayer::on_mouse_move(core::MouseMovedEvent &event) -> bool
{
    if (!m_cursor_captured)
    {
        return true;
    }

    m_camera.adjust_yaw(event.get_dx() * k_base_sensitivity * m_mouse_sensitivity);
    m_camera.adjust_pitch(-event.get_dy() * k_base_sensitivity * m_mouse_sensitivity);

    return true;
}

} // namespace ui

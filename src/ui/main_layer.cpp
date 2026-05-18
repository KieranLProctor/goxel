#include "main_layer.h"

#include "../game/camera.h"
#include "application.h"
#include "glad/glad.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "renderer/shader.h"
#include "spdlog/spdlog.h"

#include <iostream>

namespace ui
{

MainLayer::MainLayer()
{
    spdlog::info("created MainLayer!");

    float vertices[] = {
        0.0f, 0.5f, -5.0f, 1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -5.0f,
        0.0f, 1.0f, 0.0f,  0.5f, -0.5f, -5.0f, 0.0f,  0.0f,  1.0f,
    };

    m_simple_shader =
        core::renderer::create_graphics_shader("assets/shaders/simple.vert", "assets/shaders/simple.frag");

    glGenVertexArrays(1, &m_simple_vao);
    glGenBuffers(1, &m_simple_vbo);
    glBindVertexArray(m_simple_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_simple_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    m_camera.set_viewport(1000, 600);
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
    auto* window = glfwGetCurrentContext();

    glm::vec3 move_dir = {0.0f, 0.0f, 0.0f};

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) move_dir.z += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) move_dir.z -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) move_dir.x -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) move_dir.x += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)      move_dir.y += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) move_dir.y -= 1.0f;

    if (glm::length(move_dir) > 0.0f)
        move_dir = glm::normalize(move_dir);

    glm::vec3 look = m_camera.get_position();
    look += m_camera.get_front() * move_dir.z * m_move_speed * time_step;
    look += m_camera.get_right() * move_dir.x * m_move_speed * time_step;
    look += glm::vec3(0.0f, 1.0f, 0.0f) * move_dir.y * m_move_speed * time_step;
    m_camera.set_position(look);

    // m_view = m_camera.get_view_matrix();
}

auto MainLayer::on_render() -> void
{
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = m_camera.get_view_matrix();
    glm::mat4 proj = m_camera.get_projection_matrix();

    glUseProgram(m_simple_shader);
    glUniformMatrix4fv(glGetUniformLocation(m_simple_shader, "uModel"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_simple_shader, "uView"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_simple_shader, "uProjection"), 1, GL_FALSE, &proj[0][0]);

    glBindVertexArray(m_simple_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glUseProgram(0);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // --- ImGui debug overlay ---
    ImGui::Begin("Camera Debug");

    // Eye position
    auto eye = m_camera.get_position();
    ImGui::SeparatorText("Position");
    ImGui::Text("Eye:   %.2f, %.2f, %.2f", eye.x, eye.y, eye.z);

    // Direction vectors
    auto look  = m_camera.get_look();
    auto front = m_camera.get_front();
    auto right = m_camera.get_right();
    auto up    = m_camera.get_up();
    ImGui::SeparatorText("Vectors");
    ImGui::Text("Look:  %.2f, %.2f, %.2f", look.x,  look.y,  look.z);
    ImGui::Text("Front: %.2f, %.2f, %.2f", front.x, front.y, front.z);
    ImGui::Text("Right: %.2f, %.2f, %.2f", right.x, right.y, right.z);
    ImGui::Text("Up:    %.2f, %.2f, %.2f", up.x,    up.y,    up.z);

    // Yaw / pitch - expose these via getters if you don't have them
    ImGui::SeparatorText("Orientation");
    ImGui::Text("Yaw:   %.2f", m_camera.get_yaw());
    ImGui::Text("Pitch: %.2f", m_camera.get_pitch());

    // Mouse deltas - add these members to main_layer.h
    ImGui::SeparatorText("Mouse");
    // ImGui::Text("Last dX: %.4f", m_last_dx);
    // ImGui::Text("Last dY: %.4f", m_last_dy);
    ImGui::Text("Sensitivity: %.4f", m_mouse_sensitivity);
    ImGui::SliderFloat("##sens", &m_mouse_sensitivity, 0.001f, 1.0f);

    // Triangle position for reference
    ImGui::SeparatorText("Triangle");
    ImGui::Text("Position: 0.0, 0.0, -5.0");
    ImGui::Text("Dist to eye: %.2f", glm::length(eye - glm::vec3(0.0f, 0.0f, -5.0f)));

    // Camera mode
    ImGui::SeparatorText("Mode");
    const char* mode_str = "Unknown";
    switch (m_camera.get_mode())
    {
    case game::CameraMode::FIRST_PERSON:      mode_str = "First Person";       break;
    case game::CameraMode::THIRD_PERSON:  mode_str = "Third Person Back";  break;
    case game::CameraMode::THIRD_PERSON_SELF: mode_str = "Third Person Front"; break;
    }
    ImGui::Text("Mode: %s", mode_str);
    if (ImGui::Button("Cycle Mode (F5)")) m_camera.cycle_mode();

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
    auto *window = glfwGetCurrentContext();

    if (event.get_key_code() == GLFW_KEY_F5)
    {
        m_camera.cycle_mode();
    }

    if (event.get_key_code() == GLFW_KEY_ESCAPE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    return true;
}

// auto MainLayer::on_mouse_input(core::MouseButtonEvent &event) -> bool
// {
//     return true;
// }

auto MainLayer::on_mouse_move(core::MouseMovedEvent &event) -> bool
{
    m_camera.adjust_yaw  ( event.get_dx() * m_mouse_sensitivity);
    m_camera.adjust_pitch(-event.get_dy() * m_mouse_sensitivity);

    return true;
}

} // namespace ui

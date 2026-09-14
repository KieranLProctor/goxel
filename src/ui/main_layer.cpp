#include "main_layer.h"

#include "application.h"
#include "components/renderable.h"
#include "components/transform.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "imgui.h"
#include "shader.h"
#include "spdlog/spdlog.h"

namespace goxel::ui
{

MainLayer::MainLayer()
{
    spdlog::info("created MainLayer");

    constexpr float vertices[] = {
        // Front face (+Z)
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        // Back face (-Z)
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        // Right face (+X)
         1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,

        // Left face (-X)
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,

        // Top face (+Y)
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        // Bottom face (-Y)
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
    };

    const GLuint simple_shader =
        rendering::create_graphics_shader("assets/shaders/simple.vert", "assets/shaders/simple.frag");

    if (simple_shader == 0)
    {
        return;
    }

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    auto &registry = game::get_registry();

    const auto frame_buffer = core::Application::get().get_framebuffer_size();

    auto &camera = registry.ctx().emplace<rendering::Camera>(-90.0f, 0.0f);
    camera.set_viewport(static_cast<int>(frame_buffer.x), static_cast<int>(frame_buffer.y));

    const auto grass = registry.create();
    registry.emplace<game::components::Transform>(grass, glm::vec3(0.0f, 0.0f, 0.0f));
    registry.emplace<game::components::Renderable>(grass, voxel::Block::GRASS);

    // Dirt below
    const auto dirt = registry.create();
    registry.emplace<game::components::Transform>(dirt, glm::vec3(3.0f, 0.0f, 0.0f));
    registry.emplace<game::components::Renderable>(dirt, voxel::Block::DIRT);

    // Stone
    const auto stone = registry.create();
    registry.emplace<game::components::Transform>(stone, glm::vec3(6.0f, 0.0f, 0.0f));
    registry.emplace<game::components::Renderable>(stone, voxel::Block::STONE);

    m_render_system.set_shader(simple_shader);
    m_render_system.set_vao(m_vao);
    m_render_system.set_vbo(m_vbo);
    m_render_system.init();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // const auto frame_buffer = core::Application::get().get_framebuffer_size();
    // m_camera.set_viewport(static_cast<int>(frame_buffer.x), static_cast<int>(frame_buffer.y));
}

MainLayer::~MainLayer()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

auto MainLayer::on_update(const float time_step) -> void
{
    auto *window = core::Application::get().get_window()->get_handle();

    auto &camera = game::get_registry().ctx().get<rendering::Camera>();

    glm::vec3 move(0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        move += camera.get_front();
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        move -= camera.get_front();
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        move -= camera.get_right();
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        move += camera.get_right();
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        move += camera.get_world_up();
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        move -= camera.get_world_up();
    }

    if (glm::length(move) > 0.0f)
    {
        move = glm::normalize(move);
        camera.translate(move * m_move_speed * time_step);
    }

    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    // {
    //     move += m_camera.get_front();
    // }
    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    // {
    //     move -= m_camera.get_front();
    // }
    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    // {
    //     move -= m_camera.get_right();
    // }
    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    // {
    //     move += m_camera.get_right();
    // }
    // if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    // {
    //     move += m_camera.get_world_up();
    // }
    // if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    // {
    //     move -= m_camera.get_world_up();
    // }
    //
    // if (glm::length(move) > 0.0f)
    // {
    //     move = glm::normalize(move);
    //     m_camera.translate(move * m_move_speed * time_step);
    // }
}

auto MainLayer::on_render() -> void
{
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, m_render_wireframe ? GL_LINE : GL_FILL);

    m_render_system.update();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(0);
    glUseProgram(0);
}

auto MainLayer::on_event(core::Event &event) -> void
{
    core::EventDispatcher dispatcher(event);
    dispatcher.dispatch<core::KeyPressedEvent>([this](core::KeyPressedEvent &e) { return on_keyboard_input(e); });
    // dispatcher.dispatch<core::MouseButtonEvent>([this](core::MouseButtonEvent &e) { return on_mouse_input(e); });
    dispatcher.dispatch<core::MouseMovedEvent>([this](core::MouseMovedEvent &e) { return on_mouse_move(e); });
}

auto MainLayer::on_keyboard_input(const core::KeyPressedEvent &event) -> bool
{
    auto &camera = game::get_registry().ctx().get<rendering::Camera>();

    if (event.get_key_code() == GLFW_KEY_F5)
    {
        camera.cycle_mode();
    }

    if (event.get_key_code() == GLFW_KEY_ESCAPE || event.get_key_code() == GLFW_KEY_E)
    {
        m_cursor_captured = !m_cursor_captured;
        core::Application::get().get_window()->set_cursor_captured(m_cursor_captured);
    }

    // TEMP.
    if (event.get_key_code() == GLFW_KEY_UP)
    {
        camera.set_zoom(camera.get_zoom() + 1.0f);
    }

    if (event.get_key_code() == GLFW_KEY_DOWN)
    {
        camera.set_zoom(camera.get_zoom() - 1.0f);
    }

    if (event.get_key_code() == GLFW_KEY_F1)
    {
        m_render_wireframe = !m_render_wireframe;
    }

    // if (event.get_key_code() == GLFW_KEY_F5)
    // {
    //     m_camera.cycle_mode();
    // }
    //
    // if (event.get_key_code() == GLFW_KEY_ESCAPE || event.get_key_code() == GLFW_KEY_E)
    // {
    //     m_cursor_captured = !m_cursor_captured;
    //     core::Application::get().get_window()->set_cursor_captured(m_cursor_captured);
    // }
    //
    // // TEMP.
    // if (event.get_key_code() == GLFW_KEY_UP)
    // {
    //     m_camera.set_zoom(m_camera.get_zoom() + 1.0f);
    // }
    //
    // if (event.get_key_code() == GLFW_KEY_DOWN)
    // {
    //     m_camera.set_zoom(m_camera.get_zoom() - 1.0f);
    // }

    return true;
}

auto MainLayer::on_mouse_move(const core::MouseMovedEvent &event) -> bool
{
    auto &camera = game::get_registry().ctx().get<rendering::Camera>();

    if (!m_cursor_captured)
    {
        return true;
    }

    camera.adjust_yaw(static_cast<float>(event.get_dx() * k_base_sensitivity * m_mouse_sensitivity));
    camera.adjust_pitch(static_cast<float>(-event.get_dy() * k_base_sensitivity * m_mouse_sensitivity));

    // m_camera.adjust_yaw(static_cast<float>(event.get_dx() * k_base_sensitivity * m_mouse_sensitivity));
    // m_camera.adjust_pitch(static_cast<float>(-event.get_dy() * k_base_sensitivity * m_mouse_sensitivity));

    return true;
}

} // namespace ui

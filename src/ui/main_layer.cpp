#include "main_layer.h"

#include "application.h"
#include "chunk.h"
#include "chunk_mesher.h"
#include "components/chunk_mesh.h"
#include "components/transform.h"
#include "glad/glad.h"
#include "shader.h"
#include "spdlog/spdlog.h"

namespace goxel::ui
{

MainLayer::MainLayer()
{
    spdlog::info("created MainLayer");

    const GLuint simple_shader =
        rendering::create_graphics_shader("assets/shaders/simple.vert", "assets/shaders/simple.frag");

    if (simple_shader == 0)
    {
        return;
    }

    auto &registry = game::get_registry();

    const auto frame_buffer = core::Application::get().get_framebuffer_size();

    auto &camera = registry.ctx().emplace<rendering::Camera>(rendering::Camera::k_default_yaw, rendering::Camera::k_default_pitch);
    camera.set_position(rendering::Camera::k_default_position);
    camera.set_viewport(static_cast<int>(frame_buffer.x), static_cast<int>(frame_buffer.y));

    // TEMP: hardcoded layered terrain until real world generation exists.
    voxel::Chunk chunk;
    for (int x = 0; x < voxel::S_CHUNK_SIZE; ++x)
    {
        for (int y = 0; y < voxel::S_CHUNK_SIZE; ++y)
        {
            for (int z = 0; z < voxel::S_CHUNK_SIZE; ++z)
            {
                auto block = voxel::Block::AIR;

                if (y < 4)
                {
                    block = voxel::Block::STONE;
                }
                else if (y < 7)
                {
                    block = voxel::Block::DIRT;
                }
                else if (y == 7)
                {
                    block = voxel::Block::GRASS;
                }

                chunk.set_block(glm::vec3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)), block);
            }
        }
    }

    const std::vector<float> mesh_data = voxel::build_chunk_mesh(chunk);
    const int vertex_count = static_cast<int>(mesh_data.size() / 6);
    const rendering::MeshHandle mesh_handle =
        m_render_system.get_mesh_registry().create_mesh(mesh_data.data(), vertex_count);

    const auto terrain = registry.create();
    registry.emplace<game::components::Transform>(terrain, chunk.position);
    registry.emplace<game::components::ChunkMesh>(terrain, mesh_handle);

    m_render_system.set_shader(simple_shader);
    m_render_system.init();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

MainLayer::~MainLayer() = default;

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
    dispatcher.dispatch<core::KeyPressedEvent>([this](const core::KeyPressedEvent &e) { return on_keyboard_input(e); });
    // dispatcher.dispatch<core::MouseButtonEvent>([this](const core::MouseButtonEvent &e) { return on_mouse_input(e); });
    dispatcher.dispatch<core::MouseMovedEvent>([this](const core::MouseMovedEvent &e) { return on_mouse_move(e); });
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

    if (event.get_key_code() == GLFW_KEY_F5)
    {
        camera.cycle_mode();
    }

    return true;
}

auto MainLayer::on_mouse_move(const core::MouseMovedEvent &event) const -> bool
{
    auto &camera = game::get_registry().ctx().get<rendering::Camera>();

    if (!m_cursor_captured)
    {
        return true;
    }

    camera.adjust_yaw(static_cast<float>(event.get_dx() * k_base_sensitivity * m_mouse_sensitivity));
    camera.adjust_pitch(static_cast<float>(-event.get_dy() * k_base_sensitivity * m_mouse_sensitivity));

    return true;
}

} // namespace ui

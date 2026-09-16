#include "overlay_layer.h"

#include "GLFW/glfw3.h"
#include "camera.h"
#include "ecs.h"
#include "imgui.h"
#include "spdlog/spdlog.h"

namespace goxel::ui
{

OverlayLayer::OverlayLayer()
{
    spdlog::info("created OverlayLayer");
};

auto OverlayLayer::on_event(core::Event &event) -> void
{
    core::EventDispatcher dispatcher(event);
    dispatcher.dispatch<core::MouseButtonPressedEvent>([this](core::MouseButtonPressedEvent &e) -> bool
                                                       { return on_mouse_button_input(e); });
    dispatcher.dispatch<core::KeyPressedEvent>([this](core::KeyPressedEvent &e) -> bool
                                               { return on_keyboard_input(e); });
}

auto OverlayLayer::on_update(float time_step) -> void {}

auto OverlayLayer::on_render() -> void
{
    auto &camera = game::get_registry().ctx().get<rendering::Camera>();

    show_menu_bar(m_show_menu_bar);

    // --- ImGui debug overlay ---
    ImGui::Begin("Camera Debug");

    // Eye position
    const auto eye = camera.get_position();
    ImGui::SeparatorText("Position");
    ImGui::Text("Eye:   %.2f, %.2f, %.2f", eye.x, eye.y, eye.z);

    // Direction vectors
    const auto look = camera.get_look();
    const auto front = camera.get_front();
    const auto right = camera.get_right();
    const auto up = camera.get_up();
    const auto world_up = camera.get_world_up();
    ImGui::SeparatorText("Vectors");
    ImGui::Text("Look:  %.2f, %.2f, %.2f", look.x, look.y, look.z);
    ImGui::Text("Front: %.2f, %.2f, %.2f", front.x, front.y, front.z);
    ImGui::Text("Right: %.2f, %.2f, %.2f", right.x, right.y, right.z);
    ImGui::Text("Up:    %.2f, %.2f, %.2f", up.x, up.y, up.z);
    ImGui::Text("W Up:  %.2f, %.2f, %.2f", world_up.x, world_up.y, world_up.z);

    // View stuff
    ImGui::SeparatorText("View");

    auto fov = camera.get_fov();
    // ImGui::Text("FOV: %.2f", fov);
    if (ImGui::SliderFloat("FOV", &fov, 1.0f, 170.0f))
    {
        camera.set_fov(fov);
    }

    auto zoom = camera.get_zoom();
    // ImGui::Text("Zoom: %.2f", camera.get_zoom());
    if (ImGui::SliderFloat("ZOOM", &zoom, 1.0f, 10.0f))
    {
        camera.set_zoom(zoom);
    }

    ImGui::Text("Near plane: %.2f", camera.get_near_plane());
    ImGui::Text("Far plane: %.2f", camera.get_far_plane());

    // Yaw / pitch - expose these via getters if you don't have them
    ImGui::SeparatorText("Orientation");
    ImGui::Text("Yaw:   %.2f", camera.get_yaw());
    ImGui::Text("Pitch: %.2f", camera.get_pitch());

    // Camera mode
    ImGui::SeparatorText("Mode");
    auto mode_str = "Unknown";
    switch (camera.get_mode())
    {
    case rendering::CameraMode::FIRST_PERSON: mode_str = "First Person"; break;
    case rendering::CameraMode::THIRD_PERSON: mode_str = "Third Person Back"; break;
    case rendering::CameraMode::THIRD_PERSON_SELF: mode_str = "Third Person Front"; break;
    }
    ImGui::Text("Mode: %s", mode_str);
    if (ImGui::Button("Cycle Mode (F5)"))
    {
        camera.cycle_mode();
    }
    ImGui::End();
}

auto OverlayLayer::show_menu_bar(const bool is_visible) -> void
{
    if (!is_visible)
    {
        return;
    }

    auto &camera = game::get_registry().ctx().get<rendering::Camera>();

    // Menubar (same as context menu).
    if (ImGui::BeginMainMenuBar())
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.83f, 0.69f, 0.22f, 1.0f));
        ImGui::TextUnformatted("[DEVTOOLS]");
        ImGui::PopStyleColor();

        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("New World");
            ImGui::MenuItem("Open World");
            ImGui::MenuItem("Open Recent");
            ImGui::MenuItem("Save");
            ImGui::MenuItem("Save As");

            ImGui::Separator();

            ImGui::MenuItem("Exit");

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Editors"))
        {
            ImGui::MenuItem("Project Editor");
            ImGui::MenuItem("Biomes Editor");
            ImGui::MenuItem("Input Device Editor");
            ImGui::MenuItem("Input Profile Editor");
            ImGui::MenuItem("Material Editor");
            ImGui::MenuItem("Visual Effects Editor");

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Options"))
        {
            ImGui::MenuItem("User Interface");
            ImGui::MenuItem("Manage Windows");

            ImGui::Separator();

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("World"))
        {
            ImGui::MenuItem("Regenerate Terrain");
            ImGui::MenuItem("Reload Chunks");
            ImGui::MenuItem("Clear World");

            ImGui::Separator();

            // ImGui::Checkbox("Enable Chunk Streaming", &streaming);
            // ImGui::SliderInt("Render Distance", &renderDistance, 2, 32);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Rendering"))
        {
            // ImGui::Checkbox("Wireframe", &wireframe);
            // ImGui::Checkbox("VSync", &vsync);

            ImGui::Separator();

            // ImGui::Checkbox("SSAO", &ssao);
            // ImGui::Checkbox("Bloom", &bloom);

            ImGui::Separator();

            // ImGui::SliderFloat("Exposure", &exposure, 0.1f, 5.0f);
            // ImGui::SliderFloat("Gamma", &gamma, 1.0f, 3.0f);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Tools"))
        {
            // ImGui::MenuItem("Chunk Inspector", nullptr, &showChunkInspector);
            // ImGui::MenuItem("Noise Editor", nullptr, &showNoiseEditor);
            // ImGui::MenuItem("Texture Atlas Viewer", nullptr, &showAtlasViewer);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Camera"))
        {
            // ImGui::SliderFloat("Speed", &cameraSpeed, 0.1f, 50.0f);
            // ImGui::Checkbox("Lock Y Axis", &lockY);

            if (ImGui::MenuItem("Reset Position"))
            {
                camera.reset();
            }
            if (ImGui::MenuItem("Cycle Mode"))
            {
                camera.cycle_mode();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Simulation"))
        {
            // ImGui::Checkbox("Pause Simulation", &paused);
            ImGui::MenuItem("Step One Frame");

            ImGui::Separator();
            // ImGui::Checkbox("Gravity", &gravityEnabled);
            // ImGui::SliderFloat("Gravity Strength", &gravity, -50.0f, -5.0f);
            // ImGui::Checkbox("Entity Collision", &entityCollision);
            // ImGui::Checkbox("Block Physics", &blockPhysics); // falling sand, liquids etc.

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Assets"))
        {
            ImGui::MenuItem("Reload Shaders");
            ImGui::MenuItem("Reload Textures");
            ImGui::MenuItem("Reload All Assets");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Debug"))
        {
            // ImGui::Checkbox("Show Chunk Borders", &showChunkBorders);
            // ImGui::Checkbox("Show AABBs", &showAABBs);
            // ImGui::Checkbox("Show Normals", &showNormals);

            ImGui::Separator();

            // ImGui::Checkbox("Freeze Frustum", &freezeFrustum);
            // ImGui::Checkbox("Disable Culling", &disableCulling);

            ImGui::Separator();

            // ImGui::Text("FPS: %.1f", fps);
            // ImGui::Text("Chunks: %d", chunkCount);
            // ImGui::Text("Draw Calls: %d", drawCalls);

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

auto OverlayLayer::on_mouse_button_input(const core::MouseButtonPressedEvent &event) -> bool
{
    const ImGuiIO &io = ImGui::GetIO();

    if (const auto event_type = event.get_event_type();
        event_type == core::EventType::MOUSE_BUTTON_PRESSED && io.WantCaptureMouse ||
        event_type == core::EventType::MOUSE_BUTTON_RELEASED && io.WantCaptureMouse)
    {
        return true;
    }

    return false;
}

auto OverlayLayer::on_keyboard_input(const core::KeyPressedEvent &event) -> bool
{
    if (event.get_key_code() == GLFW_KEY_LEFT_ALT && !event.is_repeat())
    {
        m_show_menu_bar = !m_show_menu_bar;

        return true;
    }

    return false;
}

} // namespace ui

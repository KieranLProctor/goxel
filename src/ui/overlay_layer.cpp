#include "overlay_layer.h"

#include "../core/application.h"
#include "../core/input_event.h"
#include "GLFW/glfw3.h"
#include "imgui.h"

namespace ui
{

OverlayLayer::OverlayLayer() {}

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
    show_menu_bar(m_show_menu_bar);
}

auto OverlayLayer::show_menu_bar(bool is_visible) -> void
{
    if (!is_visible)
    {
        return;
    }

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

            ImGui::MenuItem("Reset Position");

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

auto OverlayLayer::on_mouse_button_input(core::MouseButtonPressedEvent &event) -> bool
{
    ImGuiIO &io = ImGui::GetIO();

    auto event_type = event.get_event_type();
    if (event_type == core::EventType::MOUSE_BUTTON_PRESSED && io.WantCaptureMouse ||
        event_type == core::EventType::MOUSE_BUTTON_RELEASED && io.WantCaptureMouse)
    {
        return true;
    }

    return false;
}

auto OverlayLayer::on_keyboard_input(core::KeyPressedEvent &event) -> bool
{
    if (event.get_key_code() == GLFW_KEY_LEFT_ALT && !event.is_repeat())
    {
        m_show_menu_bar = !m_show_menu_bar;

        return true;
    }

    return false;
}

} // namespace ui

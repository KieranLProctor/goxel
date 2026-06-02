#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace goxel::rendering
{

class ImGuiContext
{
  public:
    explicit ImGuiContext(GLFWwindow *window)
    {
        // Setup Dear ImGui context.
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();
    }

    ~ImGuiContext()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    ImGuiContext(const ImGuiContext &) = delete;
    ImGuiContext &operator=(const ImGuiContext &) = delete;

    auto begin_frame() -> void
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
    auto end_frame() -> void
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    auto apply_dark_theme() -> void
    {
        auto &style = ImGui::GetStyle();
        auto &colours = style.Colors;

        // ====================== Style Geometry ======================
        style.WindowRounding = 5.0f;
        style.ChildRounding = 3.0f;
        style.FrameRounding = 3.0f;
        style.PopupRounding = 4.0f;
        style.ScrollbarRounding = 6.0f;
        style.GrabRounding = 3.0f;
        style.TabRounding = 3.0f;

        style.WindowBorderSize = 0.0f;
        style.ChildBorderSize = 1.0f;
        style.FrameBorderSize = 0.0f;
        style.PopupBorderSize = 1.0f;

        style.WindowPadding = ImVec2(10, 10);
        style.FramePadding = ImVec2(8, 5);
        style.ItemSpacing = ImVec2(8, 5);
        style.ItemInnerSpacing = ImVec2(6, 4);

        // ====================== Colors ======================

        // Text
        colours[ImGuiCol_Text] = ImVec4(0.96f, 0.97f, 0.99f, 1.00f);
        colours[ImGuiCol_TextDisabled] = ImVec4(0.38f, 0.38f, 0.42f, 1.00f);

        // Backgrounds
        colours[ImGuiCol_WindowBg] = ImVec4(0.000f, 0.000f, 0.000f, 1.00f);
        colours[ImGuiCol_ChildBg] = ImVec4(0.010f, 0.010f, 0.012f, 1.00f);
        colours[ImGuiCol_PopupBg] = ImVec4(0.005f, 0.005f, 0.008f, 1.00f);
        colours[ImGuiCol_MenuBarBg] = ImVec4(0.012f, 0.012f, 0.015f, 1.00f);

        // Borders
        colours[ImGuiCol_Border] = ImVec4(0.18f, 0.18f, 0.20f, 0.55f);
        colours[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        // Title Bar
        colours[ImGuiCol_TitleBg] = ImVec4(0.000f, 0.000f, 0.000f, 1.00f);
        colours[ImGuiCol_TitleBgActive] = ImVec4(0.018f, 0.018f, 0.022f, 1.00f);
        colours[ImGuiCol_TitleBgCollapsed] = ImVec4(0.000f, 0.000f, 0.000f, 0.90f);

        // Frames & Fields
        colours[ImGuiCol_FrameBg] = ImVec4(0.055f, 0.055f, 0.065f, 1.00f);
        colours[ImGuiCol_FrameBgHovered] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
        colours[ImGuiCol_FrameBgActive] = ImVec4(0.13f, 0.13f, 0.16f, 1.00f);

        // Headers (Collapsing, Table headers, etc.)
        colours[ImGuiCol_Header] = ImVec4(0.09f, 0.10f, 0.13f, 0.70f);
        colours[ImGuiCol_HeaderHovered] = ImVec4(0.13f, 0.15f, 0.20f, 0.80f);
        colours[ImGuiCol_HeaderActive] = ImVec4(0.16f, 0.18f, 0.24f, 1.00f);

        // Buttons - Clearly visible
        colours[ImGuiCol_Button] = ImVec4(0.14f, 0.16f, 0.22f, 1.00f);
        colours[ImGuiCol_ButtonHovered] = ImVec4(0.22f, 0.25f, 0.35f, 1.00f);
        colours[ImGuiCol_ButtonActive] = ImVec4(0.28f, 0.32f, 0.45f, 1.00f);

        // Tabs
        colours[ImGuiCol_Tab] = ImVec4(0.04f, 0.04f, 0.05f, 1.00f);
        colours[ImGuiCol_TabHovered] = ImVec4(0.13f, 0.15f, 0.20f, 1.00f);
        colours[ImGuiCol_TabActive] = ImVec4(0.09f, 0.10f, 0.13f, 1.00f);

        // Accent (Bright Cyan - excellent for debug tools)
        colours[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.78f, 1.00f, 1.00f);
        colours[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.78f, 1.00f, 1.00f);
        colours[ImGuiCol_SliderGrabActive] = ImVec4(0.10f, 0.88f, 1.00f, 1.00f);

        // Plots & Graphs
        colours[ImGuiCol_PlotLines] = ImVec4(0.00f, 0.78f, 1.00f, 1.00f);
        colours[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 0.85f, 0.55f, 1.00f);
    }
};

} // namespace goxel::rendering

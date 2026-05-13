#pragma once

#include "glad/glad.h"
// #include <GLFW/glfw3.h>

#include <filesystem>

namespace core::renderer
{

struct Texture
{
    GLuint handle = 0;
    uint32_t width = 0;
    uint32_t height = 0;
};

struct Framebuffer
{
    GLuint handle = 0;
    Texture colour_attachment;
};

auto create_texture(int width, int height) -> Texture;
auto load_texture(const std::filesystem::path &path) -> Texture;
auto create_framebuffer_with_texture(Texture texture) -> Framebuffer;
auto attach_texture_to_framebuffer(const Framebuffer &framebuffer, Texture texture) -> bool;
auto blit_framebuffer_to_swapchain(Framebuffer framebuffer) -> void;

} // namespace core::renderer

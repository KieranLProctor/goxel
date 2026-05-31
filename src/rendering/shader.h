#pragma once

#include <filesystem>
#include <glad/glad.h>

namespace rendering
{

auto create_compute_shader(const std::filesystem::path &path) -> GLuint;
auto reload_compute_shader(uint32_t shader_handle, const std::filesystem::path &path) -> GLuint;

auto create_graphics_shader(const std::filesystem::path &vertex_path, const std::filesystem::path &fragment_path)
    -> GLuint;
auto reload_graphics_shader(uint32_t shader_handle, const std::filesystem::path &vertex_path,
                            const std::filesystem::path &fragment_path) -> GLuint;

} // namespace rendering

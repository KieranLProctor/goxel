#pragma once

#include <filesystem>

namespace core::renderer
{

auto create_compute_shader(const std::filesystem::path &path) -> uint32_t;
auto reload_compute_shader(uint32_t shader_handle, const std::filesystem::path &path) -> uint32_t;

auto create_graphics_shader(const std::filesystem::path &vertex_path, const std::filesystem::path &fragment_path)
    -> uint32_t;
auto reload_graphics_shader(uint32_t shader_handle, const std::filesystem::path &vertex_path,
                            const std::filesystem::path &fragment_path) -> uint32_t;

} // namespace core::renderer

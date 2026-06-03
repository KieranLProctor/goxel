#include "shader.h"

#include "glad/glad.h"
#include "spdlog/spdlog.h"

#include <fstream>
#include <vector>

namespace goxel::rendering
{

auto read_text_file(const std::filesystem::path &path) -> std::string
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        spdlog::error("Failed to open file: {}", path.string());

        return {};
    }

    return {std::istreambuf_iterator(file), {}};
}

auto create_compute_shader(const std::filesystem::path &path) -> GLuint
{
    std::string shader_source = read_text_file(path);
    if (shader_source.empty())
    {
        return 0;
    }

    GLuint shader_handle = glCreateShader(GL_COMPUTE_SHADER);

    const GLchar *source = shader_source.c_str();
    glShaderSource(shader_handle, 1, &source, nullptr);

    glCompileShader(shader_handle);

    GLint is_compiled = 0;
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE)
    {
        GLint max_length = 0;
        glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(max_length);
        glGetShaderInfoLog(shader_handle, max_length, &max_length, &info_log[0]);

        spdlog::error(info_log.data());

        glDeleteShader(shader_handle);

        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, shader_handle);
    glLinkProgram(program);

    GLint is_linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, static_cast<int *>(&is_linked));
    if (is_linked == GL_FALSE)
    {
        GLint max_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(max_length);
        glGetProgramInfoLog(program, max_length, &max_length, &info_log[0]);

        spdlog::error(info_log.data());

        glDeleteProgram(program);
        glDeleteShader(shader_handle);

        return 0;
    }

    glDetachShader(program, shader_handle);
    glDeleteShader(shader_handle);

    return program;
}

auto reload_compute_shader(uint32_t shader_handle, const std::filesystem::path &path) -> GLuint
{
    GLuint new_shader_handle = create_compute_shader(path);

    // Return old shader if compilation failed
    if (new_shader_handle == 0)
    {
        spdlog::warn("Shader reload failed, keeping old shader");

        return shader_handle;
    }

    glDeleteProgram(shader_handle);

    return new_shader_handle;
}

auto create_graphics_shader(const std::filesystem::path &vertex_path, const std::filesystem::path &fragment_path)
    -> GLuint
{
    std::string vertex_shader_source = read_text_file(vertex_path);
    std::string fragment_shader_source = read_text_file(fragment_path);

    if (vertex_shader_source.empty() || fragment_shader_source.empty())
    {
        return 0;
    }

    // Vertex shader

    GLuint vertex_shader_handle = glCreateShader(GL_VERTEX_SHADER);

    const GLchar *source = vertex_shader_source.c_str();
    glShaderSource(vertex_shader_handle, 1, &source, nullptr);

    glCompileShader(vertex_shader_handle);

    GLint is_compiled = 0;
    glGetShaderiv(vertex_shader_handle, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE)
    {
        GLint max_length = 0;
        glGetShaderiv(vertex_shader_handle, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(max_length);
        glGetShaderInfoLog(vertex_shader_handle, max_length, &max_length, &info_log[0]);

        spdlog::error(info_log.data());

        glDeleteShader(vertex_shader_handle);

        return 0;
    }

    // Fragment shader

    GLuint fragment_shader_handle = glCreateShader(GL_FRAGMENT_SHADER);

    source = fragment_shader_source.c_str();
    glShaderSource(fragment_shader_handle, 1, &source, nullptr);

    glCompileShader(fragment_shader_handle);

    is_compiled = 0;
    glGetShaderiv(fragment_shader_handle, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE)
    {
        GLint max_length = 0;
        glGetShaderiv(fragment_shader_handle, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(max_length);
        glGetShaderInfoLog(fragment_shader_handle, max_length, &max_length, &info_log[0]);

        spdlog::error(info_log.data());

        glDeleteShader(vertex_shader_handle);
        glDeleteShader(fragment_shader_handle);

        return 0;
    }

    // Program linking

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader_handle);
    glAttachShader(program, fragment_shader_handle);
    glLinkProgram(program);

    GLint is_linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, static_cast<int *>(&is_linked));
    if (is_linked == GL_FALSE)
    {
        GLint max_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(max_length);
        glGetProgramInfoLog(program, max_length, &max_length, &info_log[0]);

        spdlog::error(info_log.data());

        glDeleteProgram(program);
        glDeleteShader(vertex_shader_handle);
        glDeleteShader(fragment_shader_handle);

        return 0;
    }

    glDetachShader(program, vertex_shader_handle);
    glDetachShader(program, fragment_shader_handle);
    glDeleteShader(vertex_shader_handle);
    glDeleteShader(fragment_shader_handle);

    return program;
}

auto reload_graphics_shader(uint32_t shader_handle, const std::filesystem::path &vertex_path,
                            const std::filesystem::path &fragment_path) -> GLuint
{
    GLuint new_shader_handle = create_graphics_shader(vertex_path, fragment_path);

    // Return old shader if compilation failed
    if (new_shader_handle == 0)
    {
        spdlog::warn("Shader reload failed, keeping old shader");

        return shader_handle;
    }

    glDeleteProgram(shader_handle);

    return new_shader_handle;
}

} // namespace goxel::rendering

#include "renderer.h"

#include "spdlog/spdlog.h"
#include "stb_image.h"

namespace core::renderer
{

auto create_texture(int width, int height) -> Texture
{
    Texture result;
    result.width = width;
    result.height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &result.handle);

    glTextureStorage2D(result.handle, 1, GL_RGBA32F, width, height);

    glTextureParameteri(result.handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(result.handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(result.handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(result.handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return result;
}

auto load_texture(std::filesystem::path &path) -> Texture
{
    int width, height, channels;
    std::string filepath = path.string();
    stbi_set_flip_vertically_on_load(true);

    return texture;
}

auto create_framebuffer_with_texture(Texture texture) -> Framebuffer
{
    Framebuffer result;

    glCreateFramebuffers(1, &result.handle);

    if (!attach_texture_to_framebuffer(result, texture))
    {
        glDeleteFramebuffers(1, &result.handle);

        return {};
    }

    return result;
}

auto attach_texture_to_framebuffer(Framebuffer &framebuffer, Texture texture) -> bool
{
    glNamedFramebufferTexture(framebuffer.handle, GL_COLOR_ATTACHMENT0, texture.handle, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        spdlog::error("Framebuffer is not complete!");

        return false;
    }

    return true;
}

} // namespace core::renderer

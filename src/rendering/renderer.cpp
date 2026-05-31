#include "renderer.h"

#include "spdlog/spdlog.h"

// TODO: Improve this.
#define STB_IMAGE_IMPLEMENTATION
#include "../../external/stb/stb_image.h"

namespace rendering
{

auto create_texture(const int width, const int height, const GLenum internal_format = GL_RGBA8) -> Texture
{
    Texture result;
    result.width = width;
    result.height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &result.handle);

    glTextureStorage2D(result.handle, 1, internal_format, width, height);

    glTextureParameteri(result.handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(result.handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(result.handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(result.handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return result;
}

auto load_texture(const std::filesystem::path &path) -> Texture
{
    int width;
    int height;
    int channels;
    std::string filepath = path.string();
    stbi_set_flip_vertically_on_load(1);

    unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    if (data == nullptr)
    {
        spdlog::error("Failed to load texture: {}", filepath);

        return {};
    }

    GLenum base_format, internal_format;
    switch (channels) {
    case 4: base_format = GL_RGBA; internal_format = GL_RGBA8; break;
    case 3: base_format = GL_RGB;  internal_format = GL_RGB8;  break;
    case 1: base_format = GL_RED;  internal_format = GL_R8;    break;
    default: spdlog::error("Unsupported channel count: {}", channels); stbi_image_free(data); return {};
    }

    Texture result;
    result.width = width;
    result.height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &result.handle);

    glTextureStorage2D(result.handle, 1, internal_format, width, height);

    glTextureStorage2D(result.handle, 1, internal_format, width, height);
    glTextureSubImage2D(result.handle, 0, 0, 0, width, height, base_format, GL_UNSIGNED_BYTE, data);

    glTextureParameteri(result.handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(result.handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(result.handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(result.handle, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateTextureMipmap(result.handle);
    stbi_image_free(data);

    return result;
}

auto create_framebuffer_with_texture(const Texture texture) -> Framebuffer
{
    Framebuffer result;

    glCreateFramebuffers(1, &result.handle);

    if (!attach_texture_to_framebuffer(result, texture))
    {
        glDeleteFramebuffers(1, &result.handle);

        return {};
    }

    result.colour_attachment = texture;

    return result;
}

auto attach_texture_to_framebuffer(const Framebuffer &framebuffer, const Texture texture) -> bool
{
    glNamedFramebufferTexture(framebuffer.handle, GL_COLOR_ATTACHMENT0, texture.handle, 0);

    if (glCheckNamedFramebufferStatus(framebuffer.handle, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        spdlog::error("Framebuffer is not complete!");

        return false;
    }

    return true;
}

auto blit_framebuffer_to_swapchain(const Framebuffer framebuffer) -> void
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer.handle);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(0, 0, framebuffer.colour_attachment.width, framebuffer.colour_attachment.height, 0, 0,
                      framebuffer.colour_attachment.width, framebuffer.colour_attachment.height, GL_COLOR_BUFFER_BIT,
                      GL_NEAREST);
}

} // namespace rendering

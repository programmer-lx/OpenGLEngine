#pragma once

#include <glad/glad.h>

#include <memory>
#include <array>
#include <cinttypes>

class GBuffer
{
private:
    // Textures
    // 0: gPosition (RGB -> XYZ) (A -> linearDepth, from camera's near plane to fragment's view position)
    // 1: gNormal (RGB -> world space normal xyz)
    // 2: gAlbedo (RGB) + gSpecular (A)
    // 3: gEmission (RGB)
    std::array<std::uint32_t, 4> m_TexIDs{};

    // Framebuffer
    std::uint32_t m_FBO{};

    // Depth Renderbuffer
    std::uint32_t m_DepthRBO{};

    std::int32_t m_Width = -1;
    std::int32_t m_Height = -1;

private:
    void setBuffers(std::int32_t width, std::int32_t height);

public:
    static std::shared_ptr<GBuffer> create(std::int32_t width, std::int32_t height);
    GBuffer(std::int32_t width, std::int32_t height);
    ~GBuffer();

    void bind();
    void unbind();

    std::uint32_t fbo() const { return m_FBO; }
    std::int32_t width() const { return m_Width; }
    std::int32_t height() const { return m_Height; }

    const std::array<std::uint32_t, 4>& texIDs() const { return m_TexIDs; }
};

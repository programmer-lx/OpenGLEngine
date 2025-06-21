#pragma once

#include <glad/glad.h>

#include <cinttypes>

enum class TextureType
{
    RT,
    _2D,
    Cube
};

class Texture
{
protected:
    bool m_IsValid = false;
    TextureType m_TextureType = TextureType::_2D;

    std::uint32_t m_TexID{};

    GLint m_SelfFormat;

    GLint m_WrapMode;
    GLint m_FilterMode;

    GLint m_MipLevel = 0;

    std::int32_t m_Width = -1;
    std::int32_t m_Height = -1;
    std::int32_t m_Channels = -1;

protected:
    explicit Texture(GLint selfFormat = GL_RGBA, GLint wrapMode = GL_CLAMP_TO_EDGE, GLint filterMode = GL_LINEAR, GLint mipLevel = 0);

public:
    virtual ~Texture();

public:
    [[nodiscard]]
    bool isValid() const;

    std::int32_t width() const;
    std::int32_t height() const;

    [[nodiscard]]
    std::uint32_t texID() const;

    TextureType textureType() const;
};

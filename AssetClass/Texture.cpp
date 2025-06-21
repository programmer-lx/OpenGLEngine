#include "Texture.h"

Texture::Texture(GLint selfFormat, GLint wrapMode, GLint filterMode, GLint mipLevel)
{
    m_SelfFormat = selfFormat;
    m_WrapMode = wrapMode;
    m_FilterMode = filterMode;
    m_MipLevel = mipLevel;

    glGenTextures(1, &m_TexID);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_TexID);
}

bool Texture::isValid() const
{
    return m_IsValid;
}

std::int32_t Texture::width() const
{
    return m_Width;
}

std::int32_t Texture::height() const
{
    return m_Height;
}

std::uint32_t Texture::texID() const
{
    return m_TexID;
}

TextureType Texture::textureType() const
{
    return m_TextureType;
}

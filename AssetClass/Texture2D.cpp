#include "Texture2D.h"

#include "Misc/Debug.h"

#include <stb/stb_image.h>

Texture2D::Texture2D(GLint selfFormat, GLint wrapMode, GLint filterMode, GLint mipLevel)
    : Texture(selfFormat, wrapMode, filterMode, mipLevel)
{
    m_TextureType = TextureType::_2D;
}

std::shared_ptr<Texture2D> Texture2D::create(GLint selfFormat, GLint wrapMode, GLint filterMode, GLint mipLevel)
{
    return std::shared_ptr<Texture2D>(new Texture2D(selfFormat, wrapMode, filterMode, mipLevel));
}

Texture2D::~Texture2D()
{
    Debug::print("Texture2D destruct");
}

bool Texture2D::loadFromFile(const std::string& filePath, bool verticalFlip)
{
    // open file
    stbi_set_flip_vertically_on_load(verticalFlip);
    std::uint8_t* imgData = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_Channels, 0);
    if (!imgData)
    {
        m_IsValid = false;
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, m_TexID);

    // set params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_WrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_WrapMode);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_FilterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_FilterMode);

    // src format
    GLenum srcFormat = GL_RGBA;
    switch (m_Channels)
    {
        case 4:
            // srcFormat = GL_RGBA;
            break;

        case 3:
            srcFormat = GL_RGB;
            break;

        case 2:
            srcFormat = GL_RG;
            break;

        case 1:
            srcFormat = GL_RED;
            break;

        default:
            break;
    }

    // gen texture
    glTexImage2D(GL_TEXTURE_2D, m_MipLevel, m_SelfFormat, m_Width, m_Height, 0, srcFormat, GL_UNSIGNED_BYTE, imgData);
    glGenerateMipmap(GL_TEXTURE_2D);

    // free image data
    stbi_image_free(imgData);

    m_IsValid = true;
    return true;
}

void Texture2D::createEmptyTexture(std::int32_t width, std::int32_t height, GLint format)
{
    m_Width = width;
    m_Height = height;

    glBindTexture(GL_TEXTURE_2D, m_TexID);

    // set params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_WrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_WrapMode);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_FilterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_FilterMode);

    // gen texture
    glTexImage2D(GL_TEXTURE_2D, m_MipLevel, m_SelfFormat, m_Width, m_Height, 0, format, GL_FLOAT, nullptr);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture2D::createFromBytes(std::int32_t widthAndHeight, const float* buffer, GLint format)
{
    m_Width = widthAndHeight;
    m_Height = widthAndHeight;

    glBindTexture(GL_TEXTURE_2D, m_TexID);

    // set params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_WrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_WrapMode);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_FilterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_FilterMode);

    // gen texture
    glTexImage2D(GL_TEXTURE_2D, m_MipLevel, m_SelfFormat, m_Width, m_Height, 0, format, GL_FLOAT, buffer);
    glGenerateMipmap(GL_TEXTURE_2D);
}

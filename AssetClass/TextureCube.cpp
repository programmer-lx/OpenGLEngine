#include "TextureCube.h"

#include "Misc/Debug.h"

#include <stb/stb_image.h>

TextureCube::TextureCube(GLint selfFormat, GLint wrapMode, GLint filterMode)
    : Texture(selfFormat, wrapMode, filterMode, 0)
{
    m_TextureType = TextureType::Cube;
}

TextureCube::~TextureCube()
{
    Debug::print("TextureCube destruct");
}

std::shared_ptr<TextureCube> TextureCube::create(GLint selfFormat, GLint wrapMode, GLint filterMode)
{
    std::shared_ptr<TextureCube> ptr(new TextureCube(selfFormat, wrapMode, filterMode));
    return ptr;
}

bool TextureCube::loadFromFile(const std::array<std::string, 6>& filePath, bool verticalFlip)
{
    // bind
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TexID);

    // generate 6 texture
    for (int i = 0; i < 6; ++i)
    {
        // open file
        stbi_set_flip_vertically_on_load(verticalFlip);
        std::uint8_t* imgData = stbi_load(filePath[i].c_str(), &m_Width, &m_Height, &m_Channels, 0);
        if (!imgData)
        {
            m_IsValid = false;
            return false;
        }

        // src format
        GLenum srcFormat = GL_RGBA;
        if (m_Channels == 3)
            srcFormat = GL_RGB;

        // gen texture
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_SelfFormat, m_Width, m_Height, 0, srcFormat, GL_UNSIGNED_BYTE, imgData);

        // free image data
        stbi_image_free(imgData);
    }

    // set params
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_WrapMode);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_WrapMode);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_WrapMode);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_FilterMode);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_FilterMode);

    // unbind
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    m_IsValid = true;
    return true;
}

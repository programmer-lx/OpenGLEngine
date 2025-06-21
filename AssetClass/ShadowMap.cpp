#include "ShadowMap.h"

#include "Misc/Debug.h"
#include "Misc/Screen.h"

std::shared_ptr<ShadowMap> ShadowMap::create(std::int32_t width, std::int32_t height)
{
    auto ptr = std::shared_ptr<ShadowMap>(new ShadowMap);
    ptr->setBuffer(width, height);
    return ptr;
}

ShadowMap::ShadowMap()
{
    glGenFramebuffers(1, &m_FBO);

    m_TextureType = TextureType::RT;
}

ShadowMap::~ShadowMap()
{
    deleteBuffers();
}

void ShadowMap::setBuffer(std::int32_t width, std::int32_t height)
{
    // cache width and height
    m_Width = width;
    m_Height = height;

    // bind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    // generate a Texture2D attach to framebuffer
    glBindTexture(GL_TEXTURE_2D, m_TexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_TexID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // check state
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        Debug::print("framebuffer error");
        m_IsValid = false;
    }

    m_IsValid = true;

    // unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::bindRead()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO);
}

void ShadowMap::deleteBuffers()
{
    glDeleteFramebuffers(1, &m_FBO);

    m_IsValid = false;
}

void ShadowMap::bindWrite()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
}

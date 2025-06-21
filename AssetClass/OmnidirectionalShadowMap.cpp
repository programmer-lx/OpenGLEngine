#include "OmnidirectionalShadowMap.h"

#include "Misc/Debug.h"

void OmnidirectionalShadowMap::setBuffer(std::int32_t width, std::int32_t height)
{
    // bind
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TexID);

    // generate tex2D
    for (int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    // attach to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    // must use glFramebufferTexture        NOT glFramebufferTexture2D !!!
    // use Texture, we can use the gl_Layer variable
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_TexID, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);


    // parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // check state
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        Debug::print("framebuffer error");
        m_IsValid = false;
    }

    m_IsValid = true;

    // unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

std::shared_ptr<OmnidirectionalShadowMap> OmnidirectionalShadowMap::create(std::int32_t width, std::int32_t height)
{
    std::shared_ptr<OmnidirectionalShadowMap> ptr(new OmnidirectionalShadowMap);

    ptr->setBuffer(width, height);

    ptr->m_Width = width;
    ptr->m_Height = height;

    return ptr;
}

OmnidirectionalShadowMap::OmnidirectionalShadowMap()
{
    glGenFramebuffers(1, &m_FBO);
    m_TextureType = TextureType::Cube;
}

OmnidirectionalShadowMap::~OmnidirectionalShadowMap()
{
    glDeleteFramebuffers(1, &m_FBO);
}

void OmnidirectionalShadowMap::bindWrite()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
}

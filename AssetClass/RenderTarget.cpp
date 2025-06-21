#include "RenderTarget.h"

#include "Misc/Debug.h"
#include "Misc/Screen.h"

#include <glad/glad.h>

RenderTarget::RenderTarget()
{
    glGenFramebuffers(1, &m_FBO);
    glGenRenderbuffers(1, &m_RBO);

    m_TextureType = TextureType::RT;
}

void RenderTarget::setBuffers(std::int32_t width, std::int32_t height)
{
    if (width <= 0 || height <= 0)
    {
        width = Screen::width();
        height = Screen::height();
    }

    // cache width and height
    m_Width = width;
    m_Height = height;

    // bind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    // generate a Texture2D attach to framebuffer
    glBindTexture(GL_TEXTURE_2D, m_TexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexID, 0);

    // bind RenderBuffer
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

    // check state
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        Debug::print("framebuffer error");
        m_IsValid = false;
        deleteBuffers();
    }

    m_IsValid = true;

    // unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderTarget::deleteBuffers()
{
    glDeleteFramebuffers(1, &m_FBO);
    glDeleteRenderbuffers(1, &m_RBO);

    m_IsValid = false;
}

RenderTarget::~RenderTarget()
{
    deleteBuffers();
}

std::shared_ptr<RenderTarget> RenderTarget::create(std::int32_t width, std::int32_t height)
{
    std::shared_ptr<RenderTarget> ptr(new RenderTarget);

    // generate buffers
    ptr->setBuffers(width, height);

    return ptr;
}

void RenderTarget::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void RenderTarget::bindRead()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO);
}

void RenderTarget::bindWrite()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
}

void RenderTarget::bindDefault()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::bindDefaultRead()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void RenderTarget::bindDefaultWrite()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

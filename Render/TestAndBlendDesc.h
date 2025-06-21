#pragma once

#include <glad/glad.h>

#include <cinttypes>

struct CullDesc
{
    bool enableCull = true;
    GLenum cullFace = GL_BACK;
    GLenum frontFaceType = GL_CCW;
};

struct ColorMask
{
    bool r = true;
    bool g = true;
    bool b = true;
    bool a = true;
};

struct BlendDesc
{
    // blend functions
    GLenum srcFactor_RGB = GL_ONE;
    GLenum dstFactor_RGB = GL_ZERO;
    GLenum srcFactor_A = GL_ONE;
    GLenum dstFactor_A = GL_ZERO;

    // blend operation
    GLenum blendOperation_RGB = GL_FUNC_ADD;
    GLenum blendOperation_A = GL_FUNC_ADD;
};

struct DepthDesc
{
    bool depthTest = true;
    bool depthWrite = true;
    GLenum depthFunc = GL_LESS;
};

struct StencilDesc
{
    // Func
    GLenum compareFunc = GL_ALWAYS;
    std::uint8_t ref = 1;

    // stencilOp
    GLenum stencilFail = GL_KEEP;
    GLenum depthFail = GL_KEEP;
    GLenum pass = GL_KEEP;

    // 8-bit stencil value

    std::uint8_t writeMask = 0xff; // 255: update the stencil-buffer fully
    std::uint8_t readMask = 0xff; // 255: update the stencil-buffer fully
};

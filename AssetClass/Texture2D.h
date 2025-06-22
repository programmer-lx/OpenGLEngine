#pragma once

#include "Texture.h"

#include <glad/glad.h>

#include <memory>
#include <string>
#include <filesystem>

class Texture2D : public Texture
{
private:
    explicit Texture2D(GLint selfFormat = GL_RGBA, GLint wrapMode = GL_CLAMP_TO_EDGE, GLint filterMode = GL_LINEAR, GLint mipLevel = 0);
public:
    static std::shared_ptr<Texture2D> create(GLint selfFormat = GL_RGBA, GLint wrapMode = GL_CLAMP_TO_EDGE, GLint filterMode = GL_LINEAR, GLint mipLevel = 0);
    virtual ~Texture2D() override;

    bool loadFromFile(const std::filesystem::path& filePath, bool verticalFlip = true);
    void createEmptyTexture(std::int32_t width, std::int32_t height, GLint format);
    void createFromBytes(std::int32_t widthAndHeight, const float* buffer, GLint format);
};

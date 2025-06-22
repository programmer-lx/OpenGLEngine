#pragma once

#include "Texture.h"

#include <glad/glad.h>

#include <memory>
#include <array>
#include <string>
#include <cinttypes>
#include <filesystem>

class TextureCube : public Texture
{
private:
    TextureCube() = default;
    explicit TextureCube(GLint selfFormat = GL_RGBA, GLint wrapMode = GL_CLAMP_TO_EDGE, GLint filterMode = GL_LINEAR);

public:
    virtual ~TextureCube() override;

    static std::shared_ptr<TextureCube> create(GLint selfFormat = GL_RGBA, GLint wrapMode = GL_CLAMP_TO_EDGE, GLint filterMode = GL_LINEAR);

    /**
     * @param filePath [0]: right; [1]: left; [2]:top; [3]: down; [4]: front; [5]: back
     * @param verticalFlip vertical flip
     * @return load successfully: true, else: false
     */
    bool loadFromFile(const std::array<std::filesystem::path, 6>& filePath, bool verticalFlip = true);
};

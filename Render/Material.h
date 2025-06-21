#pragma once

#include "RenderPass.h"

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>

#include <memory>
#include <map>
#include <string>
#include <vector>

enum class MaterialType
{
    DeferredOpaque,
    ForwardOpaque,
    ForwardAlphaTest,
    ForwardTransparent
};

class Shader;
class Texture;

class Material final
{
    friend class RenderPipelineSubsystem;
    friend class Graphics;

private:
    MaterialType m_Type = MaterialType::DeferredOpaque;

    // set uniforms commands
    std::map<std::string, GLint> m_IntCommands;
    std::map<std::string, GLfloat> m_FloatCommands;
    std::map<std::string, glm::vec3> m_Float3Commands;
    std::map<std::string, glm::vec4> m_Float4Commands;
    std::map<std::string, glm::mat3x3> m_Matrix3x3Commands;
    std::map<std::string, glm::mat4x4> m_Matrix4x4Commands;
    std::map<std::string, std::shared_ptr<Texture>> m_TextureCommands;

    // Render Passes
    std::vector<std::vector<RenderPass>> m_RenderPasses;

private:
    void useShaderProgram(const RenderPass& pass);

    void executeTestAndBlendCommands(const RenderPass& pass);

    void executeAllShaderUniformCommands(const RenderPass& pass);

    void clearAllShaderUniformCommands();

    void executePostRenderCommands();

    std::vector<RenderPass>* renderPasses(const std::int16_t& passID);

private:
    Material() = default;

public:
    static std::shared_ptr<Material> create(const std::vector<RenderPass>& passes, MaterialType type = MaterialType::DeferredOpaque);

    Material(const Material&) = delete;

    Material(Material&&) = delete;

    ~Material();

    // ======================================= getters =======================================
    MaterialType materialType() const
    {
        return m_Type;
    }

    // ======================================= Shader Uniforms Functions =======================================
    void setInt(const std::string& name, GLint value);

    void setFloat(const std::string& name, GLfloat value);

    void setFloat3(const std::string& name, const glm::vec3& vec3);

    void setFloat4(const std::string& name, const glm::vec4& vec4);

    void setMatrix3x3(const std::string& name, const glm::mat3x3& mat);

    void setMatrix4x4(const std::string& name, const glm::mat4x4& mat);

    void setTexture(const std::string& name, const std::shared_ptr<Texture>& tex);
};

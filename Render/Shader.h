#pragma once

#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <cinttypes>
#include <string>
#include <memory>
#include <set>
#include <map>

class Texture;
class RenderTarget;
class Texture2D;
class TextureCube;

class Shader
{
    friend class Material;

private:
    static std::uint64_t s_ID;
    std::uint64_t m_ID;

    std::uint32_t m_ProgramID{};
    bool m_IsValid = false;

    std::set<std::shared_ptr<Texture>> m_Textures;
    std::int32_t m_NextUnit = 0;
    std::map<GLint, GLint> m_ID_Unit_Map; // uniform id -> texture unit

private:
    Shader();
    Shader(const char* vertexPath, const char* fragmentPath, bool isNativeShader);
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath, bool isNativeShader);
    Shader(const char* dirPath, bool isNativeShader);
public:
    // ============================================== constructor ==============================================
    static std::shared_ptr<Shader> create(const std::string& vertexPath, const std::string& fragmentPath);
    static std::shared_ptr<Shader> create(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath);
    static std::shared_ptr<Shader> create(const std::string& dirPath);

    static std::shared_ptr<Shader> createNative(const std::string& vertexPath, const std::string& fragmentPath);
    static std::shared_ptr<Shader> createNative(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath);
    static std::shared_ptr<Shader> createNative(const std::string& dirPath);
    ~Shader();

    bool operator==(const Shader& rhs) const;
    bool operator<(const Shader& rhs) const;
    bool operator>(const Shader& rhs) const;

    // ============================================== setter ==============================================
    void setBool(GLint id, bool value) const;
    void setBool(const char* name, bool value) const;

    void setInt(GLint id, GLint value) const;
    void setInt(const char* name, GLint value) const;

    void setFloat(GLint id, GLfloat value) const;
    void setFloat(const char* name, GLfloat value) const;

    void setFloat3(GLint id, const glm::vec3& vec3) const;
    void setFloat3(const char* name, const glm::vec3& vec3) const;

    void setFloat4(GLint id, const glm::vec4& vec4) const;
    void setFloat4(const char* name, const glm::vec4& vec4) const;

    void setMatrix3x3(GLint id, const glm::mat3x3& mat) const;
    void setMatrix3x3(const char* name, const glm::mat3x3& mat) const;

    void setMatrix4x4(GLint id, const glm::mat4x4& mat) const;
    void setMatrix4x4(const char* name, const glm::mat4x4& mat) const;

    void setTexture(GLint id, std::uint32_t texID, GLenum texTarget);
    void setTexture(const char* name, std::uint32_t texID, GLenum texTarget);
    void setTexture(GLint id, const std::shared_ptr<Texture>& tex);
    void setTexture(const char* name, const std::shared_ptr<Texture>& tex);

    void bindTextureUnit(const char* name, std::int32_t unit);

    void useProgram() const;

    // ============================================== getter ==============================================
    [[nodiscard]]
    std::uint64_t shaderID() const
    {
        return m_ID;
    }

    [[nodiscard]]
    bool isValid() const
    {
        return m_IsValid;
    }

    bool hasVariable(const char* name) const;

    GLint uniformVarNameToID(const char* name) const;
};

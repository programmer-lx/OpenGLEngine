#include "Shader.h"
#include "ShaderUtil.h"
#include "AssetClass/Texture.h"

#include "Misc/Debug.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <filesystem>

std::uint64_t Shader::s_ID = 0;

Shader::Shader()
{
    m_ID = s_ID;
    ++s_ID;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, bool isNativeShader)
    : Shader()
{
    int success = 0;

    // compile vertex shader and check
    std::uint32_t vert;
    if (!ShaderUtil::compileShader(vertexPath, GL_VERTEX_SHADER, &vert, isNativeShader))
    {
        m_IsValid = false;
        return;
    }

    // compile fragment shader and check
    std::uint32_t frag;
    if (!ShaderUtil::compileShader(fragmentPath, GL_FRAGMENT_SHADER, &frag, isNativeShader))
    {
        m_IsValid = false;
        return;
    }

    // create shader program and link
    std::uint32_t program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    // check program
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        m_IsValid = false;
        return;
    }

    // delete vertex and fragment shader
    glDeleteShader(vert);
    glDeleteShader(frag);

    m_IsValid = true;
    m_ProgramID = program;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath, bool isNativeShader)
    : Shader()
{
    int success = 0;

    // compile vertex shader and check
    std::uint32_t vert;
    if (!ShaderUtil::compileShader(vertexPath, GL_VERTEX_SHADER, &vert, isNativeShader))
    {
        m_IsValid = false;
        return;
    }

    // compile geometry shader and check
    std::uint32_t geo;
    if (!ShaderUtil::compileShader(geometryPath, GL_GEOMETRY_SHADER, &geo, isNativeShader))
    {
        m_IsValid = false;
        return;
    }

    // compile fragment shader and check
    std::uint32_t frag;
    if (!ShaderUtil::compileShader(fragmentPath, GL_FRAGMENT_SHADER, &frag, isNativeShader))
    {
        m_IsValid = false;
        return;
    }

    // create shader program and link
    std::uint32_t program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, geo);
    glAttachShader(program, frag);
    glLinkProgram(program);

    // check program
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        m_IsValid = false;
        return;
    }

    // delete vertex, geometry, fragment shader
    glDeleteShader(vert);
    glDeleteShader(geo);
    glDeleteShader(frag);

    m_IsValid = true;
    m_ProgramID = program;
}

Shader::Shader(const char* dirPath, bool isNativeShader)
    : Shader()
{
    m_IsValid = false;

    std::string directory = std::string(dirPath);
    std::int32_t lastSlashIndex = directory.find_last_of('/');
    std::string dirName = directory.substr(lastSlashIndex + 1);

    std::uint32_t vert;
    std::string vertPath = directory + '/' + dirName + ".vert.glsl";

    std::uint32_t frag;
    std::string fragPath = directory + '/' + dirName + ".frag.glsl";

    std::uint32_t geo;
    bool hasGeo = false;
    std::string geoPath = directory + '/' + dirName + ".geo.glsl";

    // compile vertex shader and fragment shader
    if (std::filesystem::exists(vertPath) && std::filesystem::exists(fragPath))
    {
        // vert
        if (!ShaderUtil::compileShader(vertPath.c_str(), GL_VERTEX_SHADER, &vert, isNativeShader))
        {
            return;
        }
        // frag
        if (!ShaderUtil::compileShader(fragPath.c_str(), GL_FRAGMENT_SHADER, &frag, isNativeShader))
        {
            return;
        }
    }
    else
    {
        Debug::print("File can not open: `{}`, `{}`", vertPath, fragPath);
        return;
    }

    // compile geometry shader and check
    if (std::filesystem::exists(geoPath))
    {
        hasGeo = true;
        if (!ShaderUtil::compileShader(geoPath.c_str(), GL_GEOMETRY_SHADER, &geo, isNativeShader))
        {
            return;
        }
    }

    // create shader program and link
    std::uint32_t program = glCreateProgram();

    glAttachShader(program, vert);
    glAttachShader(program, frag);
    if (hasGeo)
        glAttachShader(program, geo);

    glLinkProgram(program);

    // check program
    int success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        return;
    }

    // delete vertex, geometry, fragment shader
    glDeleteShader(vert);
    glDeleteShader(frag);

    if (hasGeo)
        glDeleteShader(geo);

    m_IsValid = true;
    m_ProgramID = program;
}

std::shared_ptr<Shader> Shader::create(const std::string& vertexPath, const std::string& fragmentPath)
{
    return std::shared_ptr<Shader>(new Shader(vertexPath.c_str(), fragmentPath.c_str(), false));
}

std::shared_ptr<Shader> Shader::create(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
{
    return std::shared_ptr<Shader>(new Shader(vertexPath.c_str(), fragmentPath.c_str(), geometryPath.c_str(), false));
}

std::shared_ptr<Shader> Shader::create(const std::string& dirPath)
{
    return std::shared_ptr<Shader>(new Shader(dirPath.c_str(), false));
}

std::shared_ptr<Shader> Shader::createNative(const std::string& vertexPath, const std::string& fragmentPath)
{
    return std::shared_ptr<Shader>(new Shader(vertexPath.c_str(), fragmentPath.c_str(), true));
}

std::shared_ptr<Shader> Shader::createNative(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
{
    return std::shared_ptr<Shader>(new Shader(vertexPath.c_str(), fragmentPath.c_str(), geometryPath.c_str(), true));
}

std::shared_ptr<Shader> Shader::createNative(const std::string& dirPath)
{
    return std::shared_ptr<Shader>(new Shader(dirPath.c_str(), true));
}

Shader::~Shader()
{
    if (isValid())
        glDeleteProgram(m_ProgramID);

    Debug::print("Shader destruct");
}

bool Shader::operator==(const Shader& rhs) const
{
    return m_ID == rhs.m_ID;
}

bool Shader::operator<(const Shader& rhs) const
{
    return m_ID < rhs.m_ID;
}

bool Shader::operator>(const Shader& rhs) const
{
    return m_ID > rhs.m_ID;
}

void Shader::setBool(GLint id, bool value) const
{
    glUniform1i(id, value ? 1 : 0);
}

void Shader::setBool(const char* name, bool value) const
{
    setBool(uniformVarNameToID(name), value);
}

bool Shader::hasVariable(const char* name) const
{
    return uniformVarNameToID(name) >= 0;
}

void Shader::setInt(GLint id, GLint value) const
{
    glUniform1i(id, value);
}

void Shader::setInt(const char* name, GLint value) const
{
    setInt(uniformVarNameToID(name), value);
}

void Shader::setFloat(GLint id, GLfloat value) const
{
    glUniform1f(id, value);
}

void Shader::setFloat(const char* name, GLfloat value) const
{
    setFloat(uniformVarNameToID(name), value);
}

void Shader::setFloat3(GLint id, const glm::vec3& vec3) const
{
    glUniform3fv(id, 1, glm::value_ptr(vec3));
}

void Shader::setFloat3(const char* name, const glm::vec3& vec3) const
{
    setFloat3(uniformVarNameToID(name), vec3);
}

void Shader::setFloat4(GLint id, const glm::vec4& vec4) const
{
    glUniform4fv(id, 1, glm::value_ptr(vec4));
}

void Shader::setFloat4(const char* name, const glm::vec4& vec4) const
{
    setFloat4(uniformVarNameToID(name), vec4);
}

void Shader::setMatrix3x3(GLint id, const glm::mat3x3& mat) const
{
    glUniformMatrix3fv(id, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMatrix3x3(const char* name, const glm::mat3x3& mat) const
{
    setMatrix3x3(uniformVarNameToID(name), mat);
}

void Shader::setMatrix4x4(GLint id, const glm::mat4x4& mat) const
{
    glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMatrix4x4(const char* name, const glm::mat4x4& mat) const
{
    setMatrix4x4(uniformVarNameToID(name), mat);
}

void Shader::setTexture(GLint id, std::uint32_t texID, GLenum texTarget)
{
    if (id < 0)
        return;

    // if the texture have not allocate unit, allocate it and record
    auto it = m_ID_Unit_Map.find(id);
    std::int32_t unit;
    if (it == m_ID_Unit_Map.end())
    {
        m_ID_Unit_Map[id] = m_NextUnit;
        unit = m_NextUnit;
        useProgram();
        setInt(id, unit);
        ++m_NextUnit;
    }
    else
    {
        unit = it->second;
    }


    glActiveTexture(GL_TEXTURE0 + unit);

    // different texture type, bind different target
    glBindTexture(texTarget, texID);
}

void Shader::setTexture(const char* name, std::uint32_t texID, GLenum texTarget)
{
    setTexture(uniformVarNameToID(name), texID, texTarget);
}

void Shader::setTexture(GLint id, const std::shared_ptr<Texture>& tex)
{
    if (!tex->isValid())
        return;

    // if the texture have not allocate unit, allocate it and record
    auto it = m_ID_Unit_Map.find(id);
    std::int32_t unit;
    if (it == m_ID_Unit_Map.end())
    {
        m_ID_Unit_Map[id] = m_NextUnit;
        unit = m_NextUnit;
        useProgram();
        setInt(id, unit);
        ++m_NextUnit;
    }
    else
    {
        unit = it->second;
    }


    glActiveTexture(GL_TEXTURE0 + unit);

    // different texture type, bind different target
    GLenum target = GL_TEXTURE_2D;
    switch (tex->textureType())
    {
        case TextureType::_2D:
        case TextureType::RT:
            // target = GL_TEXTURE_2D;
            break;

        case TextureType::Cube:
            target = GL_TEXTURE_CUBE_MAP;
            break;
    }
    glBindTexture(target, tex->texID());

    // possess
    m_Textures.insert(tex);
}

void Shader::setTexture(const char* name, const std::shared_ptr<Texture>& tex)
{
    setTexture(uniformVarNameToID(name), tex);
}

void Shader::bindTextureUnit(const char* name, std::int32_t unit)
{
    useProgram();
    setInt(uniformVarNameToID(name), unit);
}

void Shader::useProgram() const
{
    if (isValid())
        glUseProgram(m_ProgramID);
}

GLint Shader::uniformVarNameToID(const char* name) const
{
    return glGetUniformLocation(m_ProgramID, name);
}

#include "Material.h"

#include "Shader.h"
#include "AssetClass/Texture.h"

#include "Misc/Debug.h"

std::shared_ptr<Material> Material::create(const std::vector<RenderPass>& passes, MaterialType type)
{
    std::shared_ptr<Material> ptr(new Material);

    ptr->m_Type = type;

    for (auto& p : passes)
    {
        if (p.id >= ptr->m_RenderPasses.size())
        {
            ptr->m_RenderPasses.resize(p.id + 1);
        }
        ptr->m_RenderPasses[p.id].push_back(p);
    }

    return ptr;
}

void Material::useShaderProgram(const RenderPass& pass)
{
    pass.shader->useProgram();
}

void Material::executeTestAndBlendCommands(const RenderPass& pass)
{
    // cull
    if (pass.cullDesc.enableCull)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
    glFrontFace(pass.cullDesc.frontFaceType);
    glCullFace(pass.cullDesc.cullFace);

    // color mask
    glColorMask(pass.colorMask.r, pass.colorMask.g, pass.colorMask.b, pass.colorMask.a);

    // color blend
    if (m_Type == MaterialType::ForwardTransparent)
    {
        glBlendFuncSeparate(pass.blendDesc.srcFactor_RGB, pass.blendDesc.dstFactor_RGB, pass.blendDesc.srcFactor_A, pass.blendDesc.dstFactor_A);
        glBlendEquationSeparate(pass.blendDesc.blendOperation_RGB, pass.blendDesc.blendOperation_A);
    }

    // depth
    if (pass.depthDesc.depthTest)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
    glDepthMask(pass.depthDesc.depthWrite);
    glDepthFunc(pass.depthDesc.depthFunc);

    // stencil
    glStencilFunc(pass.stencilDesc.compareFunc, pass.stencilDesc.ref, pass.stencilDesc.readMask);
    glStencilOp(pass.stencilDesc.stencilFail, pass.stencilDesc.depthFail, pass.stencilDesc.pass);
    glStencilMask(pass.stencilDesc.writeMask);
}

void Material::executeAllShaderUniformCommands(const RenderPass& pass)
{
    // int
    for (auto& c : m_IntCommands)
    {
        pass.shader->setInt(c.first.c_str(), c.second);
    }

    // float
    for (auto& c : m_FloatCommands)
    {
        pass.shader->setFloat(c.first.c_str(), c.second);
    }

    // float3
    for (auto& c : m_Float3Commands)
    {
        pass.shader->setFloat3(c.first.c_str(), c.second);
    }

    // float4
    for (auto& c : m_Float4Commands)
    {
        pass.shader->setFloat4(c.first.c_str(), c.second);
    }

    // mat3x3
    for (auto& c : m_Matrix3x3Commands)
    {
        pass.shader->setMatrix3x3(c.first.c_str(), c.second);
    }

    // mat4x4
    for (auto& c : m_Matrix4x4Commands)
    {
        pass.shader->setMatrix4x4(c.first.c_str(), c.second);
    }

    // texture
    for (auto& c : m_TextureCommands)
    {
        pass.shader->setTexture(c.first.c_str(), c.second);
    }
}

void Material::clearAllShaderUniformCommands()
{
    m_IntCommands.clear();
    m_FloatCommands.clear();
    m_Float3Commands.clear();
    m_Float4Commands.clear();
    m_Matrix3x3Commands.clear();
    m_Matrix4x4Commands.clear();
    m_TextureCommands.clear();
}

void Material::executePostRenderCommands()
{
    // cull
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // color mask
    glColorMask(1, 1, 1, 1);

    // color blend
    glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ZERO);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);

    // depth
    glEnable(GL_DEPTH_TEST);
    glDepthMask(1);
    glDepthFunc(GL_LESS);

    // stencil
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

Material::~Material()
{
    Debug::print("Material destruct");
}

std::vector<RenderPass>* Material::renderPasses(const std::int16_t& passID)
{
    if (passID < 0 || passID > m_RenderPasses.size())
        return nullptr;

    return &m_RenderPasses[passID];
}

void Material::setInt(const std::string& name, GLint value)
{
    m_IntCommands.insert_or_assign(name, value);
}


void Material::setFloat(const std::string& name, GLfloat value)
{
    m_FloatCommands.insert_or_assign(name, value);
}

void Material::setFloat3(const ::std::string& name, const glm::vec3& vec3)
{
    m_Float3Commands.insert_or_assign(name, vec3);
}

void Material::setFloat4(const ::std::string& name, const glm::vec4& vec4)
{
    m_Float4Commands.insert_or_assign(name, vec4);
}

void Material::setMatrix3x3(const ::std::string& name, const glm::mat3x3& mat)
{
    m_Matrix3x3Commands.insert_or_assign(name, mat);
}

void Material::setMatrix4x4(const ::std::string& name, const glm::mat4x4& mat)
{
    m_Matrix4x4Commands.insert_or_assign(name, mat);
}

void Material::setTexture(const std::string& name, const std::shared_ptr<Texture>& tex)
{
    m_TextureCommands.insert_or_assign(name, tex);
}

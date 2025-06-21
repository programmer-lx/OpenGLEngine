#pragma once

#include "ComponentBase.h"

#include "Graphic/BufferObject.h"
#include "Graphic/VertexArrayObject.h"

#include <glm/glm.hpp>

#include <memory>

class TextureCube;
class Camera;
class Shader;

class Skybox : public ComponentBase
{
    FCOMPONENT(Skybox)

public:
    std::shared_ptr<TextureCube> m_TexCube;
    std::shared_ptr<Shader> m_Shader;
    std::unique_ptr<VertexArrayObject> m_VAO;
    std::unique_ptr<BufferObject> m_VBO;

    // mat params
    glm::vec3 m_ColorMultiply = {1, 1, 1};

protected:
    Skybox();

public:
    static std::shared_ptr<Skybox> create(const std::shared_ptr<TextureCube>& tex);

    virtual void render(Camera* camera);

    virtual void awake() override;

    virtual void onDestroy() override;

    virtual void onEnable() override;

    virtual void onDisable() override;

    virtual void onInspectorGUI() override;
};

#pragma once

#include "Component/Camera.h"
#include "Core/Scene.h"

class DirectionalLight;
class SpotLight;
class PointLight;
class Material;
class Shader;
class MeshRenderer;
class CameraMovement;
class Mesh;

class FirstScene : public Scene
{
private:
    std::shared_ptr<GameObject> sceneCameraObject;
    std::shared_ptr<GameObject> mountain2_Objs;
    std::shared_ptr<GameObject> shadowPointLightObj;
    std::shared_ptr<GameObject> tree1_Objs;

    std::shared_ptr<Material> oceanMaterial;
    std::shared_ptr<Material> batchLampMaterial;

    glm::vec4 waveFrequency = {3.5, 4.0, 0, 0};
    glm::vec4 waveAmplitude = {2, 3, 0, 0};
    glm::vec4 waveSpeed = {0.8, 0.5, 0, 0};
    glm::vec3 colorA = {0.217, 0.447, 0.868};
    glm::vec3 colorB = {0.244, 0.795, 0.887};

    glm::vec3 lampEmissionScale = {1, 0.744f, 0.328f};

private:
    void initUI();

public:
    ~FirstScene();

    virtual void awake() override;

    virtual void tick() override;

    virtual void onEditorGUI() override;
};

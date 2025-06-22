#include "FirstScene.h"

#include "Component/HDRISkybox.h"
#include "Misc/Utils.h"
#include "Render/Shader.h"
#include "Render/Material.h"
#include "AssetClass/RenderTarget.h"
#include "Component/PostProcessing.h"
#include "Component/Skybox.h"
#include "AssetClass/Texture2D.h"
#include "AssetClass/TextureCube.h"
#include "Core/Application.h"
#include "Component/Renderer/MeshRenderer.h"
#include "Component/Renderer/ModelRenderer.h"
#include "Component/Renderer/BatchModelRenderer.h"
#include "Component/Light/DirectionalLight.h"
#include "Component/Light/PointLight.h"
#include "Component/Light/SpotLight.h"
#include "AssetClass/Mesh.h"
#include "AssetClass/Model.h"
#include "Core/GameObject.h"
#include "Core/Time.h"
#include "Core/Subsystems/InputSubsystem.h"
#include "Core/Subsystems/SceneSubsystem.h"

#include "Misc/Debug.h"
#include "Misc/RenderSetting.h"
#include "Misc/Math.h"
#include "Misc/Screen.h"

#include "Game/Components/CameraMovement.h"
#include "Game/Components/PostEffect.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

#include <memory>

#include "Core/Subsystems/RenderPipelineSubsystem.h"

FirstScene::~FirstScene()
{
    // Debug::print("FirstScene destruct");
}

void FirstScene::awake()
{
    Scene::awake();

    // models
    auto mountain1 = Model::create(Application::resourcesPath() / "Models/LowPoly/Forest/Mountain1.obj", VertexDataType::Full);
    auto mountain2 = Model::create(Application::resourcesPath() / "Models/LowPoly/Forest/Mountain2.obj", VertexDataType::Full);

    auto meadowFlat1 = Model::create(Application::resourcesPath() / "Models/LowPoly/Forest/Meadow_Flat_1.obj", VertexDataType::Full);
    auto meadowFlat2 = Model::create(Application::resourcesPath() / "Models/LowPoly/Forest/Meadow_Flat_2.obj", VertexDataType::Full);
    auto meadowBumpy1 = Model::create(Application::resourcesPath() / "Models/LowPoly/Forest/Meadow_Bumpy_1.obj", VertexDataType::Full);

    auto rock4 = Model::create(Application::resourcesPath() / "Models/LowPoly/Forest/Rock4.obj", VertexDataType::Full);

    auto tree1 = Model::create(Application::resourcesPath() / "Models/LowPoly/Forest/Tree1.obj", VertexDataType::Full);
    auto tree2 = Model::create(Application::resourcesPath() / "Models/LowPoly/Forest/Tree2.obj", VertexDataType::Full);
    auto tree3 = Model::create(Application::resourcesPath() / "Models/LowPoly/Forest/Tree3.obj", VertexDataType::Full);
    auto tree4 = Model::create(Application::resourcesPath() / "Models/LowPoly/Forest/Tree4.obj", VertexDataType::Full);

    auto lamp1 = Model::create(Application::resourcesPath() / "Models/LowPoly/lamps-pack/Lamp1.obj", VertexDataType::Full);
    auto lamp2 = Model::create(Application::resourcesPath() / "Models/LowPoly/lamps-pack/Lamp2.obj", VertexDataType::Full);
    auto lamp3 = Model::create(Application::resourcesPath() / "Models/LowPoly/lamps-pack/Lamp3.obj", VertexDataType::Full);
    auto lamp4 = Model::create(Application::resourcesPath() / "Models/LowPoly/lamps-pack/Lamp4.obj", VertexDataType::Full);
    auto lamp5 = Model::create(Application::resourcesPath() / "Models/LowPoly/lamps-pack/Lamp5.obj", VertexDataType::Full);

    // meshes
    auto ocean = Utils::genPlane(2000, 2000, 150, 150);
    auto testCube = Mesh::create(Application::resourcesPath() / "Models/Box.txtm", MeshFileType::TxtMesh);

    // shadow shader
    auto cubeShadowCasterShader = Shader::create(Application::shadersPath() / "FlxShader/Shadow/BatchOminidirectionalShadowMap");

    // deferred shader
    auto gGeometryShader = Shader::create(Application::shadersPath() / "FlxShader/Deferred/GBuffer");
    auto batchGeometryShader = Shader::create(Application::shadersPath() / "FlxShader/Deferred/BatchGBuffer");
    auto gLightingShader = Shader::create(Application::shadersPath() / "FlxShader/Deferred/LightingPass");
    auto ssaoShader = Shader::create(Application::shadersPath() / "FlxShader/Deferred/SSAO");
    auto ssaoBlurShader = Shader::create(Application::shadersPath() / "FlxShader/Deferred/SSAOBlur");

    // forward shader
    auto fwdOceanShader = Shader::create(Application::shadersPath() / "FlxShader/Forward/Ocean");


    // builtin textures
    auto defaultNormalTex = Texture2D::create(GL_RGB);
    defaultNormalTex->loadFromFile(Application::resourcesPath() / "Textures/Builtin/DefaultNormal.png");

    auto whiteTex = Texture2D::create(GL_RGB);
    whiteTex->loadFromFile(Application::resourcesPath() / "Textures/Builtin/White.png");

    auto greyTex = Texture2D::create(GL_RGB);
    greyTex->loadFromFile(Application::resourcesPath() / "Textures/Builtin/64Grey.png");

    auto blackTex = Texture2D::create(GL_RGB);
    blackTex->loadFromFile(Application::resourcesPath() / "Textures/Builtin/Black.png");

    // low-poly texture
    auto lowPolyTex = Texture2D::create(GL_RGB);
    lowPolyTex->loadFromFile(Application::resourcesPath() / "Models/LowPoly/Forest/textures/PP_Color_Palette.png", false);

    // lamp texture
    auto lampTex = Texture2D::create(GL_RGB);
    lampTex->loadFromFile(Application::resourcesPath() / "Models/LowPoly/lamps-pack/textures/Lamps_low_Metal_BaseColor_2.png", false);

    auto lampEmissionTex = Texture2D::create(GL_RGB);
    lampEmissionTex->loadFromFile(Application::resourcesPath() / "Models/LowPoly/lamps-pack/textures/Emission_Final_2048_2.png", false);

    // skybox texture
    auto skyboxTex = TextureCube::create(GL_RGB);
    skyboxTex->loadFromFile({
        Application::resourcesPath() / "Textures/Cube/ulukai/corona_rt.png",
        Application::resourcesPath() / "Textures/Cube/ulukai/corona_lf.png",
        Application::resourcesPath() / "Textures/Cube/ulukai/corona_up.png",
        Application::resourcesPath() / "Textures/Cube/ulukai/corona_dn.png",
        Application::resourcesPath() / "Textures/Cube/ulukai/corona_ft.png",
        Application::resourcesPath() / "Textures/Cube/ulukai/corona_bk.png"
    }, true);

    // pass
    // point light shadow caster pass
    RenderPass batchPointLightShaderCasterPass;
    batchPointLightShaderCasterPass.id = RenderPass::PointLightShadowCaster;
    batchPointLightShaderCasterPass.shader = cubeShadowCasterShader;

    // low-poly g-pass
    RenderPass lowPolyGPass;
    lowPolyGPass.id = RenderPass::DeferredGeometry;
    lowPolyGPass.shader = gGeometryShader;

    // batch low-poly g-pass
    RenderPass batchLowPolyGPass;
    batchLowPolyGPass.id = RenderPass::DeferredGeometry;
    batchLowPolyGPass.shader = batchGeometryShader;

    // ocean forward-pass
    RenderPass fwdOceanPass;
    fwdOceanPass.id = RenderPass::ForwardBase;
    fwdOceanPass.shader = fwdOceanShader;

    // low-poly material
    auto lowPolyMaterial = Material::create({lowPolyGPass}, MaterialType::DeferredOpaque);
    lowPolyMaterial->setTexture("material.diffuseTex", lowPolyTex);
    lowPolyMaterial->setTexture("material.specularTex", greyTex);
    lowPolyMaterial->setTexture("material.normalTex", defaultNormalTex);
    lowPolyMaterial->setTexture("material.emissionTex", blackTex);
    lowPolyMaterial->setFloat3("material.emissionScale", lampEmissionScale);
    lowPolyMaterial->setFloat("material.shininess", 4);
    lowPolyMaterial->setFloat("material.bumpScale", 1);

    // batch low-poly material
    auto batchLowPolyMaterial = Material::create({batchPointLightShaderCasterPass, batchLowPolyGPass}, MaterialType::DeferredOpaque);
    batchLowPolyMaterial->setTexture("material.diffuseTex", lowPolyTex);
    batchLowPolyMaterial->setTexture("material.specularTex", greyTex);
    batchLowPolyMaterial->setTexture("material.normalTex", defaultNormalTex);
    batchLowPolyMaterial->setTexture("material.emissionTex", blackTex);
    batchLowPolyMaterial->setFloat3("material.emissionScale", lampEmissionScale);
    batchLowPolyMaterial->setFloat("material.shininess", 4);
    batchLowPolyMaterial->setFloat("material.bumpScale", 1);

    // batch lamp material
    batchLampMaterial = Material::create({batchPointLightShaderCasterPass, batchLowPolyGPass}, MaterialType::DeferredOpaque);
    batchLampMaterial->setTexture("material.diffuseTex", lampTex);
    batchLampMaterial->setTexture("material.specularTex", greyTex);
    batchLampMaterial->setTexture("material.normalTex", defaultNormalTex);
    batchLampMaterial->setTexture("material.emissionTex", lampEmissionTex);
    batchLampMaterial->setFloat3("material.emissionScale", lampEmissionScale);
    batchLampMaterial->setFloat("material.shininess", 4);
    batchLampMaterial->setFloat("material.bumpScale", 1);

    // ocean material
    oceanMaterial = Material::create({fwdOceanPass}, MaterialType::ForwardOpaque);
    oceanMaterial->setFloat4("waveFrequency", waveFrequency);
    oceanMaterial->setFloat4("waveAmplitude", waveAmplitude);
    oceanMaterial->setFloat4("waveSpeed", waveSpeed);
    oceanMaterial->setFloat3("colorA", colorA);
    oceanMaterial->setFloat3("colorB", colorB);
    oceanMaterial->setFloat("_fogDensity", 0.005);
    oceanMaterial->setFloat3("_fogColor", {0.144, 0.595, 0.687});

    // ======================================== GameObjects ========================================
    int64_t gameObjectCount = 0;
    // mountains
    auto mountain1_Objs = GameObject::create();
    mountain1_Objs->setName("mountain 1 objs");
    auto mountain1Transforms = Utils::decodeSceneDataFile(Application::resourcesPath() / "SceneData/Mountain1.transvector");
    mountain1_Objs->addComponent<BatchModelRenderer>(mountain1Transforms, mountain1, batchLowPolyMaterial);
    gameObjectCount += mountain1Transforms.size();

    mountain2_Objs = GameObject::create();
    mountain2_Objs->setName("mountain 2 objs");
    auto mountain2Transforms = Utils::decodeSceneDataFile(Application::resourcesPath() / "SceneData/Mountain2.transvector");
    mountain2_Objs->addComponent<BatchModelRenderer>(mountain2Transforms, mountain2, batchLowPolyMaterial);
    gameObjectCount += mountain2Transforms.size();

    // lands
    auto meadowFlat1_Objs = GameObject::create();
    meadowFlat1_Objs->setName("meadow flat 1 objs");
    auto meadowFlat1Transforms = Utils::decodeSceneDataFile(Application::resourcesPath() / "SceneData/Meadow_Flat_1.transvector");
    meadowFlat1_Objs->addComponent<BatchModelRenderer>(meadowFlat1Transforms, meadowFlat1, batchLowPolyMaterial);
    gameObjectCount += meadowFlat1Transforms.size();

    auto meadowFlat2_Objs = GameObject::create();
    meadowFlat2_Objs->setName("meadow flat 2 objs");
    auto meadowFlat2Transforms = Utils::decodeSceneDataFile(Application::resourcesPath() / "SceneData/Meadow_Flat_2.transvector");
    meadowFlat2_Objs->addComponent<BatchModelRenderer>(meadowFlat2Transforms, meadowFlat2, batchLowPolyMaterial);
    gameObjectCount += meadowFlat2Transforms.size();

    auto meadowBumpy1_Objs = GameObject::create();
    meadowBumpy1_Objs->setName("meadow bumpy 1 objs");
    auto meadowBumpy1Transforms = Utils::decodeSceneDataFile(Application::resourcesPath() / "SceneData/Meadow_Bumpy_1.transvector");
    meadowBumpy1_Objs->addComponent<BatchModelRenderer>(meadowBumpy1Transforms, meadowBumpy1, batchLowPolyMaterial);
    gameObjectCount += meadowBumpy1Transforms.size();

    // trees
    tree1_Objs = GameObject::create();
    tree1_Objs->setName("tree 1 objs");
    auto tree1Transforms = Utils::decodeSceneDataFile(Application::resourcesPath() / "SceneData/Tree1.transvector");
    tree1_Objs->addComponent<BatchModelRenderer>(tree1Transforms, tree1, batchLowPolyMaterial);
    gameObjectCount += tree1Transforms.size();

    auto tree2_Objs = GameObject::create();
    tree2_Objs->setName("tree 2 objs");
    auto tree2Transforms = Utils::decodeSceneDataFile(Application::resourcesPath() / "SceneData/Tree2.transvector");
    tree2_Objs->addComponent<BatchModelRenderer>(tree2Transforms, tree2, batchLowPolyMaterial);
    gameObjectCount += tree2Transforms.size();

    auto tree3_Objs = GameObject::create();
    tree3_Objs->setName("tree 3 objs");
    auto tree3Transforms = Utils::decodeSceneDataFile(Application::resourcesPath() / "SceneData/Tree3.transvector");
    tree3_Objs->addComponent<BatchModelRenderer>(tree3Transforms, tree3, batchLowPolyMaterial);
    gameObjectCount += tree3Transforms.size();

    auto tree4_Objs = GameObject::create();
    tree4_Objs->setName("tree 4 objs");
    auto tree4Transforms = Utils::decodeSceneDataFile(Application::resourcesPath() / "SceneData/Tree4.transvector");
    tree4_Objs->addComponent<BatchModelRenderer>(tree4Transforms, tree4, batchLowPolyMaterial);
    gameObjectCount += tree4Transforms.size();

    // rocks
    auto rock4_Objs = GameObject::create();
    rock4_Objs->setName("rock 4 objs");
    auto rock4Transforms = Utils::decodeSceneDataFile(Application::resourcesPath() / "SceneData/Rock4.transvector");
    rock4_Objs->addComponent<BatchModelRenderer>(rock4Transforms, rock4, batchLowPolyMaterial);
    gameObjectCount += rock4Transforms.size();

    // Lamps
    auto lamp1_Objs = GameObject::create();
    lamp1_Objs->setName("lamp 1 objs");
    auto lamp1Transforms = Utils::decodeSceneDataFile(Application::resourcesPath() / "SceneData/Lamp1.transvector");
    lamp1_Objs->addComponent<BatchModelRenderer>(lamp1Transforms, lamp1, batchLampMaterial);
    gameObjectCount += lamp1Transforms.size();

    auto lamp2_Objs = GameObject::create();
    lamp2_Objs->setName("lamp 2 objs");
    auto lamp2Transforms = Utils::decodeSceneDataFile(Application::resourcesPath() / "SceneData/Lamp2.transvector");
    lamp2_Objs->addComponent<BatchModelRenderer>(lamp2Transforms, lamp2, batchLampMaterial);
    gameObjectCount += lamp2Transforms.size();

    auto lamp3_Objs = GameObject::create();
    lamp3_Objs->setName("lamp 3 objs");
    auto lamp3Transforms = Utils::decodeSceneDataFile(Application::resourcesPath() / "SceneData/Lamp3.transvector");
    lamp3_Objs->addComponent<BatchModelRenderer>(lamp3Transforms, lamp3, batchLampMaterial);
    gameObjectCount += lamp3Transforms.size();

    auto lamp4_Objs = GameObject::create();
    lamp4_Objs->setName("lamp 4 objs");
    auto lamp4Transforms = Utils::decodeSceneDataFile(Application::resourcesPath() / "SceneData/Lamp4.transvector");
    lamp4_Objs->addComponent<BatchModelRenderer>(lamp4Transforms, lamp4, batchLampMaterial);
    gameObjectCount += lamp4Transforms.size();

    auto lamp5_Objs = GameObject::create();
    lamp5_Objs->setName("lamp 5 objs");
    auto lamp5Transforms = Utils::decodeSceneDataFile(Application::resourcesPath() / "SceneData/Lamp5.transvector");
    lamp5_Objs->addComponent<BatchModelRenderer>(lamp5Transforms, lamp5, batchLampMaterial);
    gameObjectCount += lamp5Transforms.size();

    // Ocean
    auto oceanObj = GameObject::create();
    oceanObj->setName("ocean obj");
    oceanObj->transform().setPosition({0, -7, 0});
    oceanObj->addComponent<MeshRenderer>(ocean, oceanMaterial);
    gameObjectCount += 1;

    std::cout << "GameObject Count = " << gameObjectCount << std::endl;


    // ======================================== Lights ========================================
    int64_t lightCount = 0;

    // dirLights
    auto dirLightObj = GameObject::create();
    dirLightObj->setName("Directional Light");
    dirLightObj->transform().setRotation({1, -0.25, 0, 0});
    auto dirLightComp = dirLightObj->addComponent<DirectionalLight>();
    dirLightComp->color = {0.063f, 0.047f, 0.117f};

    lightCount++;

    // pointLights
    auto pointLightTransforms = Utils::decodeSceneDataFile(Application::resourcesPath() / "SceneData/PointLight_Lamp.transvector");
    int index = 0;
    for (const auto& p : pointLightTransforms)
    {
        auto go = GameObject::create();
        go->setName("point light " + std::to_string(index++));
        go->transform().setPosition(p.position());
        auto l = go->addComponent<PointLight>();
        l->kl = 1.48f;
        l->kq = 0.4f;
        l->color = {1, 0.588f, 0};

        lightCount++;
    }

    // shadow point light
    shadowPointLightObj = GameObject::create();
    shadowPointLightObj->setName("Shadow Point Light");
    shadowPointLightObj->transform().setPosition({-309, 15, -365});
    auto light = shadowPointLightObj->addComponent<PointLight>(true);
    light->kl = 0.3f;
    light->kq = 0.2f;
    light->color = {1, 0.388f, 0.5f};
    light->projectionFar = 200;

    lightCount++;

    std::cout << "Light Count = " << lightCount << std::endl;

    // ======================================== scene Camera ========================================
    sceneCameraObject = GameObject::create();
    sceneCameraObject->setName("Scene Camera");
    sceneCameraObject->transform().setPosition(glm::vec3(26, 22, 171));
    sceneCameraObject->transform().worldRotate(1.3f, Transform::worldRight());
    sceneCameraObject->transform().worldRotate(11.65f, Transform::worldUp());

    auto cam = sceneCameraObject->addComponent<Camera>(0);
    // base params
    cam->setFov(45.0f);
    cam->setFar(10000);

    // g-lighting shader
    cam->setDeferredLightingShader(gLightingShader);

    // ssao shader & ssao blur shader
    cam->setSsaoShader(ssaoShader);
    cam->setSsaoBlurShader(ssaoBlurShader);

    // camera movement component
    sceneCameraObject->addComponent<CameraMovement>();

    // skybox
    sceneCameraObject->addComponent<Skybox>(skyboxTex);

    // post-processing
    auto rt = RenderTarget::create(1920, 1080);
    cam->setRenderTarget(rt);
    sceneCameraObject->addComponent<PostEffect>();
}

void FirstScene::tick()
{
    auto input = InputSubsystem::instance();

    // quit
    if (input->keyDown(GLFW_KEY_ESCAPE))
    {
        Application::quit();
    }

    // select
    if (input->keyDown(GLFW_KEY_RIGHT))
    {
        selectNextGameObject();
    }
    if (input->keyDown(GLFW_KEY_LEFT))
    {
        selectPreviousGameObject();
    }
    if (input->keyDown(GLFW_KEY_1))
    {
        selectGameObject(sceneCameraObject.get());
    }
    if (input->keyDown(GLFW_KEY_2))
    {
        selectGameObject(mountain2_Objs.get());
    }
    if (input->keyDown(GLFW_KEY_3))
    {
        selectGameObject(shadowPointLightObj.get());
    }
    if (input->keyDown(GLFW_KEY_4))
    {
        selectGameObject(tree1_Objs.get());
    }
}

void FirstScene::initUI()
{
}

void FirstScene::onEditorGUI()
{
    // 获取主视口信息
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 viewportSize = io.DisplaySize;

    // 计算侧边栏的位置和大小
    ImVec2 sidebarSize = ImVec2(viewportSize.x * 0.25f, viewportSize.y); // 宽度为屏幕的1/4，高度为全屏
    ImVec2 sidebarPos = ImVec2(0, 0); // 位置固定在左侧

    // 设置下一个窗口的位置和大小
    ImGui::SetNextWindowPos(sidebarPos);
    ImGui::SetNextWindowSize(sidebarSize);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove;

    // create sidebar
    ImGui::Begin("Detail", nullptr, window_flags);
    if (ImGui::DragFloat4("waveFrequency", glm::value_ptr(waveFrequency), 0.01f))
        oceanMaterial->setFloat4("waveFrequency", waveFrequency);

    if (ImGui::DragFloat4("waveAmplitude", glm::value_ptr(waveAmplitude), 0.01f))
        oceanMaterial->setFloat4("waveAmplitude", waveAmplitude);

    if (ImGui::DragFloat4("waveSpeed", glm::value_ptr(waveSpeed), 0.01f))
        oceanMaterial->setFloat4("waveSpeed", waveSpeed);

    if (ImGui::ColorEdit3("Water color A", glm::value_ptr(colorA)))
        oceanMaterial->setFloat3("colorA", colorA);

    if (ImGui::ColorEdit3("Water color B", glm::value_ptr(colorB)))
        oceanMaterial->setFloat3("colorB", colorB);

    ImGui::Separator();

    ImGui::DragFloat("Fog density", &(RenderPipelineSubsystem::instance()->m_FogDensity.x), 0.0001f, 0, 5, "%.5f");
    ImGui::ColorEdit4("Fog color", glm::value_ptr(RenderPipelineSubsystem::instance()->m_FogColor));
    ImGui::ColorEdit4("Ambient color", glm::value_ptr(RenderPipelineSubsystem::instance()->m_AmbientColor));

    ImGui::Separator();

    if (ImGui::ColorEdit3("Lamp emission scale", glm::value_ptr(lampEmissionScale)))
        batchLampMaterial->setFloat3("material.emissionScale", lampEmissionScale);
    ImGui::End();
}

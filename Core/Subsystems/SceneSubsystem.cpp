#include "SceneSubsystem.h"

#include "Core/Scene.h"

void SceneSubsystem::loadSceneDeferred()
{
    // unload previous scene
    if (m_CurScene)
    {
        m_CurScene->onDestroy();
        m_CurScene->scene_onDestroy();
        m_CurScene = nullptr;
    }

    // load new scene
    m_CurScene = std::move(m_Scene_WillLoad);
    m_Scene_WillLoad = nullptr;
    m_CurScene->awake();

    m_CanLoadScene = true;

    Debug::print("---------------------------------------\nLoad scene");
}

SceneSubsystem::~SceneSubsystem()
{
    Debug::print("SceneSubsystem destruct");
}

void SceneSubsystem::tick()
{
    if (m_Scene_WillLoad)
    {
        loadSceneDeferred();
    }

    if (m_CurScene)
    {
        m_CurScene->scene_tick();
        m_CurScene->tick();
    }
}

void SceneSubsystem::onDestroy()
{
    // destroy last scene when quitting application
    if (m_CurScene)
    {
        m_CurScene->onDestroy();
        m_CurScene->scene_onDestroy();
        m_CurScene = nullptr;
    }
}

void SceneSubsystem::addGameObject(const std::shared_ptr<GameObject>& gameObject)
{
    m_CurScene->addGameObject(gameObject);
}

const std::set<std::shared_ptr<GameObject>>& SceneSubsystem::gameObjects() const
{
    return m_CurScene->gameObjects();
}

Scene* SceneSubsystem::curScene() const
{
    return m_CurScene.get();
}

GameObject* SceneSubsystem::selectedGameObject() const
{
    return m_CurScene->selectedGameObject();
}

void SceneSubsystem::removeGameObject(const std::shared_ptr<GameObject>& gameObject)
{
    m_CurScene->removeGameObject(gameObject);
}

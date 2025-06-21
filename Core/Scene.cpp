#include "Scene.h"

#include "GameObject.h"
#include "Misc/Debug.h"

#include <algorithm>


void Scene::scene_tick()
{
    // remove gameobjects
    removeGameObjectDeferred();
    // add gameobjects
    addGameObjectDeferred();
}

void Scene::scene_onDestroy()
{
    // destroy all gameobjects
    std::for_each(m_GameObjects.begin(), m_GameObjects.end(), [](const std::shared_ptr<GameObject>& gameObject)
    {
        gameObject->onDestroy();
    });
    m_GameObjects.clear();
}

void Scene::addGameObjectDeferred()
{
    std::for_each(m_GameObjects_WillAdd.begin(), m_GameObjects_WillAdd.end(), [&](const std::shared_ptr<GameObject>& gameObject)
    {
        m_GameObjects.insert(gameObject);
        gameObject->awake();
    });
    m_GameObjects_WillAdd.clear();

    if (m_CurIt == m_GameObjects.end())
        m_CurIt = m_GameObjects.begin();
}

void Scene::removeGameObjectDeferred()
{
    std::for_each(m_GameObjects_WillDestroy.begin(), m_GameObjects_WillDestroy.end(), [&](const std::shared_ptr<GameObject>& gameObject)
    {
        gameObject->onDestroy();
        m_GameObjects.erase(gameObject);
    });
    m_GameObjects_WillDestroy.clear();

    if (m_CurIt == m_GameObjects.end())
        m_CurIt = m_GameObjects.begin();
}

Scene::~Scene()
{
    Debug::print("Scene destruct\n---------------------------------------");
}

const std::set<std::shared_ptr<GameObject>>& Scene::gameObjects() const
{
    return m_GameObjects;
}

GameObject* Scene::selectedGameObject() const
{
    return m_SelectedObject;
}

void Scene::awake()
{
    m_CurIt = m_GameObjects.begin();
}

void Scene::tick()
{
}

void Scene::onDestroy()
{
}

void Scene::onEditorGUI()
{
}

void Scene::addGameObject(const std::shared_ptr<GameObject>& gameObject)
{
    m_GameObjects_WillAdd.push_back(gameObject);
}

void Scene::removeGameObject(const std::shared_ptr<GameObject>& gameObject)
{
    m_GameObjects_WillDestroy.push_back(gameObject);
}

void Scene::selectNextGameObject()
{
    ++m_CurIt;
    if (m_CurIt == m_GameObjects.end())
        m_CurIt = m_GameObjects.begin();

    m_SelectedObject = m_CurIt->get();
}

void Scene::selectPreviousGameObject()
{
    if (m_CurIt == m_GameObjects.begin())
        m_CurIt = m_GameObjects.end();
    --m_CurIt;

    m_SelectedObject = m_CurIt->get();
}

void Scene::selectGameObject(GameObject* obj)
{
    m_SelectedObject = obj;
}

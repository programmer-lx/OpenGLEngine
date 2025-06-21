#pragma once

#include <memory>
#include <vector>
#include <set>

class GameObject;

class Scene
{
    friend class SceneSubsystem;

private:
    std::vector<std::shared_ptr<GameObject>> m_GameObjects_WillAdd;
    std::vector<std::shared_ptr<GameObject>> m_GameObjects_WillDestroy;

    std::set<std::shared_ptr<GameObject>>::iterator m_CurIt;
    GameObject* m_SelectedObject = nullptr;

    void scene_tick();
    void scene_onDestroy();

private:
    std::set<std::shared_ptr<GameObject>> m_GameObjects;

    void addGameObjectDeferred();
    void removeGameObjectDeferred();
    void addGameObject(const std::shared_ptr<GameObject>& gameObject);
    void removeGameObject(const std::shared_ptr<GameObject>& gameObject);

protected:
    void selectNextGameObject();
    void selectPreviousGameObject();
    void selectGameObject(GameObject* obj);

public:
    virtual ~Scene();

    const std::set<std::shared_ptr<GameObject>>& gameObjects() const;
    GameObject* selectedGameObject() const;

    virtual void awake();
    virtual void tick();
    virtual void onDestroy();
    virtual void onEditorGUI();
};

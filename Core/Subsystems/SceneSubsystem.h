#pragma once

#include "SubsystemBase.h"
#include "Misc/Debug.h"

#include <memory>
#include <set>

class Scene;

class SceneSubsystem : public SubsystemBase<SceneSubsystem>
{
    friend class GameObject;

private:
    std::unique_ptr<Scene> m_CurScene;

    std::unique_ptr<Scene> m_Scene_WillLoad;

    bool m_CanLoadScene = true;

    void loadSceneDeferred();
    void addGameObject(const std::shared_ptr<GameObject>& gameObject);

public:
    const std::set<std::shared_ptr<GameObject>>& gameObjects() const;
    Scene* curScene() const;
    GameObject* selectedGameObject() const;

public:
    virtual ~SceneSubsystem() override;

    virtual void tick() override;
    virtual void onDestroy() override;

    template <typename TScene>
        requires std::is_base_of_v<Scene, TScene>
    void loadScene()
    {
        if (!m_CanLoadScene)
            return;
        m_CanLoadScene = false;

        // load new scene
        m_Scene_WillLoad = std::make_unique<TScene>();
    }

    void removeGameObject(const std::shared_ptr<GameObject>& gameObject);
};

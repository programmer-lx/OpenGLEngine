#pragma once

#include "Transform.h"
#include "Component/ComponentBase.h"

#include <vector>
#include <memory>
#include <string>

class Camera;
class MeshRenderer;
class ComponentBase;

class GameObject final
{
private:
    static std::uint64_t s_ID;

private:
    std::string m_Name = "None";
    std::uint64_t m_ID;
    bool m_Enabled = true;

    Transform m_Transform;

    std::vector<std::shared_ptr<ComponentBase>> m_Components;

private:
    GameObject();

public:
    static std::shared_ptr<GameObject> create();

    ~GameObject();

    bool operator==(const GameObject& rhs) const;

    bool operator<(const GameObject& rhs) const;

    bool operator>(const GameObject& rhs) const;

    void awake();

    void fixedTick();
    void tick();

    void onDestroy();

    std::uint64_t gameObjectID() const
    {
        return m_ID;
    }

    Transform& transform()
    {
        return m_Transform;
    }

    [[nodiscard]]
    const std::string& name() const
    {
        return m_Name;
    }

    void setName(const std::string& name)
    {
        m_Name = name;
    }

    bool isEnabled() const;
    void setEnable(bool enable);

    template<typename T, typename... Args>
    T* addComponent(Args&&... args)
    {
        std::shared_ptr<T> comp = T::create(std::forward<Args>(args)...);
        m_Components.push_back(comp);

        comp->setOwner(this);
        comp->awake();
        return comp.get();
    }

    bool removeComponent(ComponentBase* component);

    template<typename T>
        requires std::is_base_of_v<ComponentBase, T>
    bool removeComponent()
    {
        auto iter = std::find_if(m_Components.begin(), m_Components.end(),
                                 [&](const std::shared_ptr<ComponentBase>& component)
                                 {
                                     return component->componentClassName() == T::staticComponentClassName();
                                 });

        if (iter != m_Components.end())
        {
            (*iter)->onDestroy();
            m_Components.erase(iter);
            return true;
        }

        return false;
    }

    template<typename T>
        requires std::is_base_of_v<ComponentBase, T>
    T* getComponent()
    {
        auto iter = std::find_if(m_Components.begin(), m_Components.end(),
                                 [&](const std::shared_ptr<ComponentBase>& component)
                                 {
                                     return component->componentClassName() == T::staticComponentClassName();
                                 });

        if (iter == m_Components.end())
            return nullptr;

        return dynamic_cast<T*>((*iter).get());
    }

    const std::vector<std::shared_ptr<ComponentBase>>& components() const;

    void drawTransformInspectorGUI();
    void drawInspectorGUI();
};

#include "Core/GameObject.h"

#include "Component/ComponentBase.h"
#include "Misc/Debug.h"
#include "Subsystems/SceneSubsystem.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>

std::uint64_t GameObject::s_ID = 0;

GameObject::GameObject()
{
    m_ID = s_ID;
    ++s_ID;
}

std::shared_ptr<GameObject> GameObject::create()
{
    std::shared_ptr<GameObject> ptr(new GameObject);
    SceneSubsystem::instance()->addGameObject(ptr);
    return ptr;
}

GameObject::~GameObject()
{
    Debug::print("GameObject destruct, name: {}", name());
}

bool GameObject::operator==(const GameObject& rhs) const
{
    return m_ID == rhs.m_ID;
}

bool GameObject::operator<(const GameObject& rhs) const
{
    return m_ID < rhs.m_ID;
}

bool GameObject::operator>(const GameObject& rhs) const
{
    return m_ID > rhs.m_ID;
}

void GameObject::awake()
{
    Debug::print("GameObject awake, name: `{}`", name());
}

void GameObject::fixedTick()
{
    for (auto& component : m_Components)
    {
        if (component->isEnabled())
            component->fixedTick();
    }
}

void GameObject::tick()
{
    for (auto& component : m_Components)
    {
        if (component->isEnabled())
            component->tick();
    }
}

void GameObject::onDestroy()
{
    std::for_each(m_Components.begin(), m_Components.end(), [](const std::shared_ptr<ComponentBase>& comp)
    {
        comp->onDestroy();
    });
    m_Components.clear();

    Debug::print("GameObject destroy, name: `{}`", name());
}

bool GameObject::isEnabled() const
{
    return m_Enabled;
}

void GameObject::setEnable(bool enable)
{
    if (m_Enabled == enable)
        return;

    m_Enabled = enable;

    for (auto& component : m_Components)
    {
        component->setEnable(enable);
    }
}

bool GameObject::removeComponent(ComponentBase* component)
{
    auto iter = std::find_if(m_Components.begin(), m_Components.end(),
                             [&](const std::shared_ptr<ComponentBase>& c)
                             {
                                 return *component == *c;
                             });

    if (iter != m_Components.end())
    {
        (*iter)->onDestroy();
        m_Components.erase(iter);
        return true;
    }

    return false;
}

const std::vector<std::shared_ptr<ComponentBase>>& GameObject::components() const
{
    return m_Components;
}

void GameObject::drawTransformInspectorGUI()
{
    // position
    glm::vec3 pos = transform().position();
    if (ImGui::DragFloat3("Position", glm::value_ptr(pos), 0.01f))
    {
        transform().setPosition(pos);
    }

    // rotation (display as quaternion)
    auto rotation = transform().rotation();
    if (ImGui::DragFloat4("Quaternion", glm::value_ptr(rotation), 0.01f))
    {
        transform().setRotation(rotation);
    }

    // scale
    glm::vec3 s = transform().scale();
    if (ImGui::DragFloat3("Scale", glm::value_ptr(s), 0.01f))
    {
        transform().setScale(s);
    }
}

void GameObject::drawInspectorGUI()
{
    // draw title
    ImGui::Text("GameObject: %s", name().c_str());
    bool enable = m_Enabled;
    if (ImGui::Checkbox("Enable", &enable))
    {
        setEnable(enable);
    }
    ImGui::Separator();

    // Draw Transform's Inspector
    std::string gobjID = "GameObject" + std::to_string(gameObjectID());
    ImGui::PushID(gobjID.c_str());
    ImGui::SeparatorText("Transform");
    drawTransformInspectorGUI();
    ImGui::PopID();

    // Components onInspectorGUI() callback
    for (const auto& c : components())
    {
        std::string compID = "Component" + std::to_string(c->componentID());
        ImGui::PushID(compID.c_str());

        if (ImGui::CollapsingHeader(c->componentClassName().c_str()))
        {
            c->onInspectorGUI();
        }

        ImGui::Spacing();

        ImGui::PopID();
    }
}

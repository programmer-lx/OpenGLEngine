#include "ComponentBase.h"

#include "Core/GameObject.h"
#include "Misc/Debug.h"

#include <imgui/imgui.h>

std::uint64_t ComponentBase::s_ID = 0;

void ComponentBase::setOwner(GameObject* ownerGameObject)
{
    m_OwnerGameObject = ownerGameObject;
}

ComponentBase::ComponentBase()
{
    m_ID = s_ID;
    ++s_ID;
}

bool ComponentBase::operator==(const ComponentBase& rhs) const
{
    return m_ID == rhs.m_ID;
}

bool ComponentBase::operator<(const ComponentBase& rhs) const
{
    return m_ID < rhs.m_ID;
}

bool ComponentBase::operator>(const ComponentBase& rhs) const
{
    return m_ID > rhs.m_ID;
}

void ComponentBase::onInspectorGUI()
{
    bool enable = m_Enabled;
    if (ImGui::Checkbox("Enable", &enable))
    {
        setEnable(enable);
    }
}

Transform& ComponentBase::transform()
{
    return m_OwnerGameObject->transform();
}

const Transform& ComponentBase::transform() const
{
    return m_OwnerGameObject->transform();
}

bool ComponentBase::isEnabled() const
{
    return m_Enabled;
}

void ComponentBase::setEnable(bool enable)
{
    if (m_Enabled == enable)
        return;

    m_Enabled = enable;

    if (m_Enabled)
        onEnable();
    else
        onDisable();
}

#include "SubsystemManager.h"

#include "Core/Subsystems/SubsystemBase.h"

void SubsystemManager::tick()
{
    for (const auto& system : m_Subsystems)
    {
        system->tick();
    }
}

void SubsystemManager::shutdown()
{
    for (const auto& system : m_Subsystems)
    {
        system->onDestroy();
    }
}

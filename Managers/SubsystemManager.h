#pragma once

#include "ManagerBase.h"

#include <vector>
#include <memory>

class AbstractSubsystem;

class SubsystemManager : public ManagerBase<SubsystemManager>
{
private:
    std::vector<std::unique_ptr<AbstractSubsystem>> m_Subsystems;

public:
    template <typename T, typename... Args>
        requires std::is_base_of_v<AbstractSubsystem, T>
    void add(Args&&... args)
    {
        m_Subsystems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    void tick();
    void shutdown();
};

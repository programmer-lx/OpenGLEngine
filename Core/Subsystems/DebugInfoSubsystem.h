#pragma once

#include "SubsystemBase.h"

#include <memory>
#include <string>

class Timer;
struct GLFWwindow;

class DebugInfoSubsystem : public SubsystemBase<DebugInfoSubsystem>
{
private:
    std::unique_ptr<Timer> m_FramerateTimer;
    GLFWwindow* m_Window = nullptr;
    std::string m_DefaultTitle;

public:
    DebugInfoSubsystem(const std::string& defaultTitle, GLFWwindow* window);

    virtual void tick() override;
};

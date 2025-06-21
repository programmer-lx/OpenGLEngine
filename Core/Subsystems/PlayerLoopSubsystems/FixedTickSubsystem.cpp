#include "FixedTickSubsystem.h"

#include "Core/Time.h"
#include "Core/GameObject.h"
#include "Core/Subsystems/SceneSubsystem.h"

float FixedTickSubsystem::accumulatedTime() const
{
    return m_AccumulatedTime;
}

void FixedTickSubsystem::tick()
{
    // when fixedDeltaTime equal to zero, stop tick
    if (Time::fixedDeltaTime() == 0.0f)
        return;

    m_AccumulatedTime += Time::deltaTime();

    SceneSubsystem* subsystem = SceneSubsystem::instance();

    std::int32_t step = 0;

    while (m_AccumulatedTime >= Time::fixedDeltaTime() && step < Time::maxFixedTickCountPerFrame())
    {
        for (const auto& g : subsystem->gameObjects())
        {
            g->fixedTick();
        }

        ++step;
        m_AccumulatedTime -= Time::fixedDeltaTime();
    }
}

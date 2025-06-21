#include "Timer.h"

#include "Core/Time.h"

Timer::Timer(float interval, const std::function<void()>& callback, bool useTimeScale, bool startImmediately)
{
    m_Interval = interval;
    m_Delegates.add(callback);
    m_UseTimeScale = useTimeScale;

    if (startImmediately)
        start();
}

void Timer::tick()
{
    if (m_IsPaused)
        return;

    m_Time += (m_UseTimeScale ? Time::deltaTime() : Time::unscaledDeltaTime());

    // time out
    if (m_Time >= m_Interval)
    {
        m_Time = 0.0f;
        m_Delegates.invoke();
    }
}

void Timer::setUseTimeScale(bool use)
{
    m_UseTimeScale = use;
}

void Timer::setInterval(float interval)
{
    m_Interval = interval;
}

void Timer::addCallback(const std::function<void()>& callback)
{
    m_Delegates.add(callback);
}

void Timer::start()
{
    m_IsPaused = false;
}

void Timer::restart()
{
    m_Time = 0.0f;
    resume();
}

void Timer::resume()
{
    m_IsPaused = false;
}

void Timer::pause()
{
    m_IsPaused = true;
}

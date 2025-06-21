#include "TimeSubsystem.h"

#include "Core/Time.h"

#include <Windows.h>

TimeSubsystem::TimeSubsystem()
{
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    m_Frequency = freq.QuadPart;

    LARGE_INTEGER start;
    QueryPerformanceCounter(&start);
    m_StartTime = start.QuadPart;
}

TimeSubsystem::~TimeSubsystem()
{
}

void TimeSubsystem::tick()
{
    LARGE_INTEGER temp;
    QueryPerformanceCounter(&temp);
    m_EndTime = temp.QuadPart;
    QueryPerformanceFrequency(&temp);
    m_Frequency = temp.QuadPart;


    // lock frame rate
    const double duration = static_cast<double>(m_EndTime - m_StartTime) * 1000.0 / static_cast<double>(m_Frequency);
    const double targetDuration = 1000.0 / m_MaxFrameRate;
    // wait
    if (duration < targetDuration)
    {
        double remaining = targetDuration - duration;
        LARGE_INTEGER waitStart;
        QueryPerformanceCounter(&waitStart);

        // 混合休眠+忙等待
        if (remaining > 2.0)
        {
            Sleep(static_cast<DWORD>(remaining - 1.0));
        }

        // 微调最后1ms
        LARGE_INTEGER now;
        do
        {
            QueryPerformanceCounter(&now);
        } while ((now.QuadPart - waitStart.QuadPart) * 1000.0 / m_Frequency < remaining);
    }


    // update end time
    QueryPerformanceCounter(&temp);
    m_EndTime = temp.QuadPart;
    // delta time
    Time::m_UnscaledDeltaTime = static_cast<float>(static_cast<double>(m_EndTime - m_StartTime) / static_cast<double>(m_Frequency));
    Time::m_DeltaTime = Time::unscaledDeltaTime() * Time::timeScale();
    // time
    Time::m_UnscaledTime += Time::unscaledDeltaTime();
    Time::m_Time += Time::deltaTime();


    // update start time
    m_StartTime = m_EndTime;
}

#include "WindowsPlatform.h"

#include "Misc/Debug.h"

#include <Windows.h>

WindowsPlatform::~WindowsPlatform()
{
    // reset to default timer
    timeEndPeriod(1);

    Debug::print("WindowsPlatform destruct");
}

WindowsPlatform& WindowsPlatform::instance()
{
    static WindowsPlatform p;
    return p;
}

void WindowsPlatform::init()
{
    // high precision timer
    TIMECAPS tc;
    timeGetDevCaps(&tc, sizeof(TIMECAPS));
    timeBeginPeriod(tc.wPeriodMin);

    Debug::print("WindowsPlatform initialized");
}

std::filesystem::path WindowsPlatform::exePath() const
{
    char path[4097]{};
    std::memset(path, '\0', 4097 * sizeof(char));
    GetModuleFileNameA(nullptr, path, 4096);

    std::string result = path;
    int len = result.size();
    for (int i = 0; i < len; ++i)
    {
        if (result[i] == '\\')
            result[i] = '/';
    }
    return result;
}

std::filesystem::path WindowsPlatform::exeDir() const
{
    auto path = exePath();
    return std::filesystem::absolute(path.parent_path());
}

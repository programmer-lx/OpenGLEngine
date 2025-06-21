#pragma once

#include <string>

class WindowsPlatform final
{
private:
    WindowsPlatform() = default;
    WindowsPlatform(const WindowsPlatform&) = delete;
    WindowsPlatform(WindowsPlatform&&) = delete;

public:
    ~WindowsPlatform();
    static WindowsPlatform& instance();

    void init();
    std::string exePath() const;
    std::string exeDir() const;
};

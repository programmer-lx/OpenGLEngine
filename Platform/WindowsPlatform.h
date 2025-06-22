#pragma once

#include <string>
#include <filesystem>

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
    std::filesystem::path exePath() const;
    std::filesystem::path exeDir() const;
};

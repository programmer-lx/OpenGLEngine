#pragma once

#include <string>
#include <filesystem>

struct GLFWwindow;

class Application
{
    static GLFWwindow* s_Window;
    static std::filesystem::path s_DataPath;

public:
    static void init(GLFWwindow* window);

    static const std::filesystem::path& dataPath();
    static std::filesystem::path shadersPath();
    static std::filesystem::path resourcesPath();

    static void quit();
};

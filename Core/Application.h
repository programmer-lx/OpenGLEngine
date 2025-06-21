#pragma once

#include <string>

struct GLFWwindow;

class Application
{
    static GLFWwindow* s_Window;
    static std::string s_DataPath;

public:
    static void init(GLFWwindow* window);

    static const std::string& dataPath();
    static std::string shadersPath();
    static std::string resourcesPath();

    static void quit();
};

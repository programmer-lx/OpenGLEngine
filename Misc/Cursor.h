#pragma once

struct GLFWwindow;

class Cursor
{
private:
    static GLFWwindow* s_Window;

public:
    static void init(GLFWwindow* window);

    static void setShowCursor(bool show);
};

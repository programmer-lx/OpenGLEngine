#pragma once

#include <glad/glad.h>

class RenderSetting
{
public:
    static void setShowWireframe(bool show)
    {
        if (show)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
};
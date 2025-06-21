#include "Debug.h"

#include <glad/glad.h>

void Debug::printShaderError(std::uint32_t shaderID, std::uint32_t shaderType, const char* filePath)
{
#if FLX_DEBUG_BUILD
    char info[512];
    glGetShaderInfoLog(shaderID, 512, nullptr, info);
    std::string type;
    switch (shaderType)
    {
        case GL_VERTEX_SHADER:
            type = "Vertex Shader";
            break;
        case GL_FRAGMENT_SHADER:
            type = "Fragment Shader";
            break;
        case GL_GEOMETRY_SHADER:
            type = "Geometry Shader";
            break;
        default:
            break;
    }

    print("Fail to compile `{}` shader, filepath: `{}`, info: `{}`", type, filePath, info);
#endif
}

#include "ShaderUtil.h"

#include "Core/Application.h"
#include "Misc/Debug.h"
#include "Misc/File.h"

std::string ShaderUtil::s_HeaderStr{};
std::string ShaderUtil::s_FunctionStr{};

bool ShaderUtil::compileShader(const char* filePath, GLenum shaderType, std::uint32_t* outID, bool isNativeShader)
{
    std::string code = std::string(GLSL_VERSION_STR) + '\n';

    // generate non-native headers
    if (!isNativeShader)
    {
        // cache header and function code
        if (s_HeaderStr.empty() || s_FunctionStr.empty())
        {
            File header(Application::shadersPath() + "/Includes/Header.glsl", std::ios::in);
            File functions(Application::shadersPath() + "/Includes/Functions.glsl", std::ios::in);

            if (!header.isValid() || !functions.isValid())
                return false;

            s_HeaderStr = header.getString();
            s_FunctionStr = functions.getString();
        }

        code += s_HeaderStr + '\n';
        code += s_FunctionStr + '\n';
    }


    // open file
    File targetFile(filePath, std::ios::in);
    if (!targetFile.isValid())
        return false;

    // read string from file
    std::string targetFileStr = targetFile.getString();
    code += targetFileStr + '\n';

    // compile shader
    std::uint32_t shaderID = glCreateShader(shaderType);
    const char* str = code.c_str();
    glShaderSource(shaderID, 1, &str, nullptr);
    glCompileShader(shaderID);

    // check
    int success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        // print error string
        Debug::printShaderError(shaderID, shaderType, filePath);

        // delete shader
        glDeleteShader(shaderID);

        return false;
    }

    // success
    *outID = shaderID;
    return true;
}

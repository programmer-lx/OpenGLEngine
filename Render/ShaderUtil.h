#pragma once

#include <glad/glad.h>

#include <cinttypes>
#include <string>

class ShaderUtil
{
private:
    static std::string s_HeaderStr;
    static std::string s_FunctionStr;

public:
    static constexpr const char* GLSL_VERSION_STR = "#version 430 core";
    static constexpr const char* GLSL_UNIFORM_BLOCK_NAME_PER_CAMERA = "FlxUniformBlock_PerCamera";
    static constexpr const char* GLSL_UNIFORM_BLOCK_NAME_PER_FRAME = "FlxUniformBlock_PerFrame";

    static constexpr const char* UNIFORM_NAME_POINT_LIGHT_COUNT = "_pointLightCount";
    static constexpr const char* UNIFORM_NAME_SPOT_LIGHT_COUNT = "_spotLightCount";
    static constexpr const char* UNIFORM_NAME_DIR_LIGHT_COUNT = "_dirLightCount";

    static constexpr const char* UNIFORM_NAME_MATRIX_M = "_matrix_M";
    static constexpr const char* UNIFORM_NAME_MATRIX_V = "_matrix_V";
    static constexpr const char* UNIFORM_NAME_MATRIX_P = "_matrix_P";
    static constexpr const char* UNIFORM_NAME_MATRIX_VP = "_matrix_VP";
    static constexpr const char* UNIFORM_NAME_MATRIX_MVP = "_matrix_MVP";
    static constexpr const char* UNIFORM_NAME_MATRIX_OBJECT_TO_WORLD_NORMAL = "_matrix_ObjectToWorldNormal";
    static constexpr const char* UNIFORM_NAME_MATRIX_OBJECT_TO_VIEW_NORMAL = "_matrix_ObjectToViewNormal";

    static constexpr const char* UNIFORM_NAME_G_POSITION = "_gPositionDepth";
    static constexpr const char* UNIFORM_NAME_G_NORMAL = "_gNormal";
    static constexpr const char* UNIFORM_NAME_G_ALBEDO_SPECULAR = "_gAlbedoSpecular";
    static constexpr const char* UNIFORM_NAME_G_EMISSION = "_gEmission";
    static constexpr const char* UNIFORM_NAME_SSAO_TEX_NO_BLUR = "_ssaoTex_NoBlur";
    static constexpr const char* UNIFORM_NAME_SSAO_TEX = "_ssaoTex";
    static constexpr const char* UNIFORM_NAME_SSAO_NOISE_TEX = "_ssaoNoiseTex";

    static constexpr const char* UNIFORM_NAME_CAMERA_POS = "_cameraPos";
    static constexpr const char* UNIFORM_NAME_TIME = "_time";
    static constexpr const char* UNIFORM_NAME_SCREEN = "_screen";
    static constexpr const char* UNIFORM_NAME_CAMERA = "_camera";

public:
    static bool compileShader(const char* filePath, GLenum shaderType, std::uint32_t* outID, bool isNativeShader);
};

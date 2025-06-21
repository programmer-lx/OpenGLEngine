
// options
#define ENABLE_SSAO 1
#define ENABLE_SHADOW_MAP 1
#define ENABLE_FOG 1


#define TRANSFORM_TEX(texCoord, st) vec2(texCoord * st.xy + st.zw)
#define AMBIENT_LIGHT _ambientColor.rgb

struct Material
{
    sampler2D diffuseTex;
    sampler2D specularTex;
    sampler2D normalTex;
    sampler2D emissionTex;
    vec3 emissionScale;
    float bumpScale;
    float shininess;
};

struct PBRMaterial
{
    sampler2D albedoTex;
    vec3 albedoStrength;

    sampler2D normalTex;

    sampler2D metallicTex;
    float metallicStrength;

    sampler2D roughnessTex;
    float roughnessStrength;

    sampler2D aoTex;
    float aoStrength;
};

struct DirLight
{
    vec4 direction;
    vec4 color;
};

struct PointLight
{
    vec4 position;
    vec4 color;

    float kc;
    float kl;
    float kq;
};

struct SpotLight
{
    vec4 position;
    vec4 direction;

    vec4 color;

    float cosInnerCone;
    float cosOuterCone;

    float kc;
    float kl;
    float kq;
};

// uniform block
layout (std140, binding = 0) uniform FlxUniformBlock_PerCamera
{
    mat4x4 _matrix_V;
    mat4x4 _matrix_P;
    mat4x4 _matrix_VP;

    mat4x4 _matrix_WorldToViewNormal;

    // x : near
    // y : far
    // z : fov
    // w : half height (orthometric)
    vec4 _camera;

    vec4 _cameraPos;
};

layout (std140, binding = 1) uniform FlxUniformBlock_PerFrame
{
    // x : Time::time()
    // y : sin(Time::time())
    // z : cos(Time::time())
    // w : 1
    vec4 _time;

    // x : screen width
    // y : screen height
    // z : 1 / width
    // w : 1 / height
    vec4 _screen;
};

layout (std140, binding = 2) uniform FlxUniformBlock_Light
{
    int _pointLightCount;
    int _spotLightCount;
    int _dirLightCount;

    int _shadowPointLightIndex;
    int _shadowDirLightIndex;
};

// ================================= SSBO =================================
layout (std430, binding = 0) buffer FlxPointLight
{
    PointLight _pointLights[32];
};

layout (std430, binding = 1) buffer FlxSpotLight
{
    SpotLight _spotLights[32];
};

layout (std430, binding = 2) buffer FlxDirLight
{
    DirLight _dirLights[2];
};

layout (std430, binding = 3) buffer FlxSSAOKernal
{
    vec4 _ssaoKernel[16];
    vec4 _ssaoKernelCount;
};

layout (std430, binding = 4) buffer FlxFog
{
    vec4 _fogDensity;
    vec4 _fogColor;
    vec4 _ambientColor;
};

uniform mat4x4 _matrix_M;
uniform mat4x4 _matrix_MVP;
uniform mat3x3 _matrix_ObjectToWorldNormal;
uniform mat3x3 _matrix_ObjectToViewNormal;

// G-Buffer uniform
uniform sampler2D _gPositionDepth;
uniform sampler2D _gNormal;
uniform sampler2D _gAlbedoSpecular;
uniform sampler2D _gEmission;

// SSAO uniform
uniform sampler2D _ssaoTex_NoBlur;
uniform sampler2D _ssaoTex;
uniform sampler2D _ssaoNoiseTex;

uniform samplerCube _skyboxTex;

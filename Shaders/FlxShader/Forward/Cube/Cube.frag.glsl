out vec4 outColor;

// in
in v2f
{
    vec2 texCoord;
    vec3 wNormal;
    vec3 wVertex;
} v2fData;

#define POINT_LIGHT_COUNT 4

uniform Material material;

uniform Light dirLight;
uniform Light pointLights[POINT_LIGHT_COUNT];
uniform Light spotLight;

uniform sampler2D clipTex;

void main()
{
    vec3 result = calcDirLight(material, v2fData.texCoord, dirLight, v2fData.wVertex, v2fData.wNormal);

    for (int i = 0; i < POINT_LIGHT_COUNT; ++i)
    {
        result += calcPointLight(material, v2fData.texCoord, pointLights[i], v2fData.wVertex, v2fData.wNormal);
    }

    result += calcSpotLight(material, v2fData.texCoord, spotLight, v2fData.wVertex, v2fData.wNormal);

    outColor = vec4(result, 1);
}
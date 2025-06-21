out vec4 outColor;

// in
in v2f
{
    vec2 texCoord;
    vec3 wNormal;
    vec3 wVertex;
} v2fData;

uniform Material material;
uniform Light pointLights[1];

uniform float useBlinnPhong;

void main()
{
    vec3 result;
    if (useBlinnPhong == 1.0f)
    {
        result = calcPointLight_Lambert_BlinnPhong(material, v2fData.texCoord, pointLights[0], v2fData.wVertex, v2fData.wNormal);
    }
    else
    {
        result = calcPointLight_Lambert_Phong(material, v2fData.texCoord, pointLights[0], v2fData.wVertex, v2fData.wNormal);
    }

    outColor = vec4(result, 1);
}
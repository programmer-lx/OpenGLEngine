in GSOUT
{
    vec3 wVertex;
} fsin;

uniform float farPlane;

void main()
{
    float dis = length(fsin.wVertex - _pointLights[_shadowPointLightIndex].position.xyz);
    gl_FragDepth = dis / farPlane;
}
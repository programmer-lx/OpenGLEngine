out vec4 outColor;

// in
in vec2 texCoord;
in vec3 wNormal;
in vec3 wVertex;

uniform samplerCube skybox;

uniform float refractRatio;

void main()
{
    vec3 dir = refract(normalize(wVertex - _cameraPos.xyz), wNormal, refractRatio);
    outColor = texture(skybox, dir);
}
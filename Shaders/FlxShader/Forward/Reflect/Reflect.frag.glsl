out vec4 outColor;

// in
in vec2 texCoord;
in vec3 wNormal;
in vec3 wVertex;

uniform samplerCube skybox;

void main()
{
    vec3 dir = reflect(normalize(wVertex - _cameraPos.xyz), wNormal);
    outColor = texture(skybox, dir);
}

out vec4 outColor;

in vec2 texCoord;

uniform sampler2D _mainTex;

void main()
{
    outColor = vec4(pow(texture(_mainTex, texCoord).xyz, vec3(1.0f / 2.2f)), 1.0);
}
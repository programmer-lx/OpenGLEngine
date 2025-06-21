in vec2 texCoord;
out vec4 outColor;

uniform sampler2D testSsaoTex;

void main()
{
    vec4 color = texture(testSsaoTex, texCoord);
    outColor = vec4(color.r, color.r, color.r, 1);
}
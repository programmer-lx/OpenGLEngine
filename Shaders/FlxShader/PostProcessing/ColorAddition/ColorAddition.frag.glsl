
out vec4 outColor;

in vec2 texCoord;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
    outColor = vec4(texture(tex1, texCoord).rgb + texture(tex2, texCoord).rgb, 1);
}
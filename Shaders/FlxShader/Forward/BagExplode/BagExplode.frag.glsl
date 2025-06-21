out vec4 outColor;

in vec2 texCoord;

uniform Material material;

void main()
{
    outColor = texture(material.diffuseTex, texCoord);
}
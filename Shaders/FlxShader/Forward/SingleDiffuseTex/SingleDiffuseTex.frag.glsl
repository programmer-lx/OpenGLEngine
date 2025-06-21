in vec2 texCoord;

out vec4 outColor;

uniform Material material;

void main()
{
    outColor = texture(material.diffuseTex, texCoord);
}
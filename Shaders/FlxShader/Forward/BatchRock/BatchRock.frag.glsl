uniform Material material;

in vec2 texCoord;

out vec4 outColor;

void main()
{
    outColor = texture(material.diffuseTex, texCoord);
}
out vec4 outColor;

in vec2 texCoord;

uniform Material material;

void main()
{
    vec4 color = texture(material.diffuseTex, texCoord);
    if (color.a <= 0.01f)
    {
        discard;
    }

    outColor = color;
}
out float outColor;

in VSOUT
{
    vec2 texCoord;
} fsIn;

const float sampleStep = 2.0f;

void main()
{
    vec2 texelSize = 1.0f / vec2(textureSize(_ssaoTex_NoBlur, 0));
    float color = 0.0f;
    for (float x = -sampleStep; x < sampleStep; ++x)
    {
        for (float y = -sampleStep; y < sampleStep; ++y)
        {
            color += texture(_ssaoTex_NoBlur, fsIn.texCoord + vec2(x, y) * texelSize).r;
        }
    }

    outColor = color / (sampleStep * sampleStep * 4.0f);
}
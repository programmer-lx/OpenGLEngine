
out vec4 outColor;

in vec2 texCoord;

uniform sampler2D screenTex;
uniform float isHorizontal;
uniform float blurOffsetMultiplication;

const float gaussianKernel[5] = float[5] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    vec2 offset = vec2(1.0) / vec2(textureSize(screenTex, 0));
    vec3 result = texture(screenTex, texCoord).rgb * gaussianKernel[0];

    // sample horizontally
    if (isHorizontal == 1.0)
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(screenTex, texCoord + vec2(offset.x * i * blurOffsetMultiplication, 0)).rgb * gaussianKernel[i];
            result += texture(screenTex, texCoord - vec2(offset.x * i * blurOffsetMultiplication, 0)).rgb * gaussianKernel[i];
        }
    }
    // sample vertically
    else
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(screenTex, texCoord + vec2(0, offset.y * i * blurOffsetMultiplication)).rgb * gaussianKernel[i];
            result += texture(screenTex, texCoord - vec2(0, offset.y * i * blurOffsetMultiplication)).rgb * gaussianKernel[i];
        }
    }

    outColor = vec4(result, 1);
}

out vec4 outColor;

in vec2 texCoord;

uniform sampler2D screenTex;
uniform float brightThreshold;

void main()
{
    vec3 color = texture(screenTex, texCoord).rgb;
    float brightness = rec709Brightness(color);

    outColor = brightness > brightThreshold ? vec4(color, 1) : vec4(0, 0, 0, 1);
}
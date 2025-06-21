
out vec4 outColor;

in vec2 texCoord;

uniform sampler2D screenTex;

uniform float gamma;

uniform float grey;
uniform float contrast;

void main()
{
    outColor = vec4(pow(texture(screenTex, texCoord).xyz, vec3(1.0f / gamma)), 1.0);
    outColor = vec4(mix(vec3(grey), outColor.xyz, contrast), 1);
}
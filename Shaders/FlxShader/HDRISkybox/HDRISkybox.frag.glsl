out vec4 outColor;

in vec3 oPos;
in vec3 texCoord;

uniform sampler2D skybox;
uniform vec3 skyboxColorMultiply;

const vec2 invAtan = vec2(0.1591, 0.3183); // 1/(2π), 1/π

vec2 sampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{
    vec3 dir = normalize(oPos);
    vec2 uv = sampleSphericalMap(dir);
    vec3 color = texture(skybox, uv).rgb;
    outColor = vec4(color * skyboxColorMultiply, 1.0);
}
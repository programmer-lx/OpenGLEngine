layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VSOUT
{
    vec2 texCoord;
} gsin[];

out vec2 texCoord;

uniform float explodeSpeed;

vec3 getNormal()
{
    vec3 a = gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz;
    vec3 b = gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz;

    return normalize(cross(a, b));
}

vec4 getNextPos(vec4 pos, vec3 normal)
{
    vec3 dir = normal * _time.x * explodeSpeed;
    return pos + vec4(dir, 0.0f);
}

void main()
{
    vec3 N = getNormal();

    gl_Position = getNextPos(gl_in[0].gl_Position, N);
    texCoord = gsin[0].texCoord;
    EmitVertex();

    gl_Position = getNextPos(gl_in[1].gl_Position, N);
    texCoord = gsin[1].texCoord;
    EmitVertex();

    gl_Position = getNextPos(gl_in[2].gl_Position, N);
    texCoord = gsin[2].texCoord;
    EmitVertex();

    EndPrimitive();
}
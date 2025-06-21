layout (location = 0) in vec3 oVertex;
layout (location = 1) in vec3 oNormal;
layout (location = 2) in vec2 uv;

out VSOUT
{
    vec2 texCoord;
    vec3 wNormal;
    vec3 wVertex;
} vsout;

void main()
{
    vsout.texCoord = uv;
    vsout.wNormal = _matrix_ObjectToWorldNormal * oNormal;
    vsout.wVertex = vec3(_matrix_M * vec4(oVertex, 1));
    gl_Position = _matrix_MVP * vec4(oVertex, 1);
}
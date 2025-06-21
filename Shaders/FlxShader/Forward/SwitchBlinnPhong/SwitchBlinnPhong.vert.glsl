layout (location = 0) in vec3 oVertex;
layout (location = 1) in vec3 oNormal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 oTangent;
layout (location = 4) in vec3 oBitangent;
layout (location = 7) in mat4x4 instanceMatrixM;

// out
out v2f
{
    vec2 texCoord;
    vec3 wNormal;
    vec3 wVertex;
} v2fData;

void main()
{
    gl_Position = _matrix_MVP * vec4(oVertex, 1.0f);
    v2fData.texCoord = uv;
    v2fData.wVertex = vec3(_matrix_M * vec4(oVertex, 1.0f));
    v2fData.wNormal = _matrix_ObjectToWorldNormal * oNormal;
}
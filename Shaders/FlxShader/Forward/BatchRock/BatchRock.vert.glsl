layout (location = 0) in vec3 oVertex;
layout (location = 1) in vec3 oNormal;
layout (location = 2) in vec2 uv;
layout (location = 7) in mat4x4 instanceMatrixM;

out vec2 texCoord;

void main()
{
    gl_Position = _matrix_MVP * instanceMatrixM * vec4(oVertex, 1);
    texCoord = uv;
}
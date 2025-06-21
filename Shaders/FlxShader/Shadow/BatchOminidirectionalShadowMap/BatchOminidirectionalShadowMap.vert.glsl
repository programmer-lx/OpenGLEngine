layout (location = 0) in vec3 oVertex;
layout (location = 1) in vec3 oNormal;
layout (location = 2) in vec2 uv;
layout (location = 7) in mat4x4 instanceMatrixM;

void main()
{
    // transform model space to world space
    mat4x4 M = _matrix_M * instanceMatrixM;
    gl_Position = M * vec4(oVertex, 1);
}
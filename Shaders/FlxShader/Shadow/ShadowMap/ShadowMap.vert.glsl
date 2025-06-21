layout (location = 0) in vec3 oVertex;
layout (location = 1) in vec3 oNormal;
layout (location = 2) in vec2 uv;

uniform mat4x4 _dirLightSpaceMatrix_VP;

void main()
{
    gl_Position = _dirLightSpaceMatrix_VP * _matrix_M * vec4(oVertex, 1);
}
layout (location = 0) in vec3 oVertex;
layout (location = 1) in vec3 oNormal;
layout (location = 2) in vec2 uv;

void main()
{
    gl_Position = _matrix_MVP * vec4(oVertex, 1);
}

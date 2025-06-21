layout (location = 0) in vec3 vertex;

void main()
{
    gl_Position = _matrix_MVP * vec4(vertex, 1.0f);
}
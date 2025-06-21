layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

void main()
{
    gl_Position = _matrix_MVP * vec4(position, 1.0f);
    vs_out.FragPos = vec3(_matrix_M * vec4(position, 1.0));
    vs_out.Normal = _matrix_ObjectToWorldNormal * normal;
    vs_out.TexCoords = texCoords;
}
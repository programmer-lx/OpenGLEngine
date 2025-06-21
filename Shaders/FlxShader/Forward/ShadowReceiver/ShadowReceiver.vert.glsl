
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 _dirLightSpaceMatrix_VP;

void main()
{
    vs_out.FragPos = vec3(_matrix_M * vec4(aPos, 1.0));
    vs_out.Normal = _matrix_ObjectToWorldNormal * aNormal;
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace = _dirLightSpaceMatrix_VP * _matrix_M * vec4(aPos, 1);
    gl_Position = _matrix_MVP * vec4(aPos, 1.0);
}
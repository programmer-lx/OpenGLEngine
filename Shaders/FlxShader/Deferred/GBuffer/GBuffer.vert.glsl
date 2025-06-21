layout (location = 0) in vec3 oVertex;
layout (location = 1) in vec3 oNormal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 oTangent;
layout (location = 4) in vec3 oBitangent;

out VSOUT
{
    vec3 wVertex; // world space vertex position
    vec2 texCoord;

} vsOut;

flat out mat3x3 T2W;

void main()
{
    vsOut.wVertex = vec3(_matrix_M * vec4(oVertex, 1));

    vec3 wTangent = mat3x3(_matrix_M) * oTangent;
    vec3 wNormal = _matrix_ObjectToWorldNormal * normalize(oNormal);
    vec3 wBitangent = mat3x3(_matrix_M) * oBitangent;
    T2W = mat3(wTangent, wBitangent, wNormal);

    vsOut.texCoord = uv;

    gl_Position = _matrix_MVP * vec4(oVertex, 1);
}
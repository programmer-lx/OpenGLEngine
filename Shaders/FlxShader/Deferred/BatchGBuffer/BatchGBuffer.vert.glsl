layout (location = 0) in vec3 oVertex;
layout (location = 1) in vec3 oNormal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 oTangent;
layout (location = 4) in vec3 oBitangent;
layout (location = 7) in mat4x4 instanceMatrixM;

out VSOUT
{
    vec3 wVertex; // world space vertex position
    vec2 texCoord;

} vsOut;

flat out mat3x3 T2W;

void main()
{
    mat4x4 M = _matrix_M * instanceMatrixM;

    vsOut.wVertex = vec3(M * vec4(oVertex, 1));

    vec3 wTangent = mat3x3(M) * oTangent;
    vec3 wNormal = mat3x3(M) * normalize(oNormal);
    vec3 wBitangent = cross(wNormal, wTangent);
    T2W = mat3(wTangent, wBitangent, wNormal);

    vsOut.texCoord = uv;

    gl_Position = _matrix_VP * M * vec4(oVertex, 1);
}
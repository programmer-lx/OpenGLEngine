layout (location = 0) in vec3 oVertex;
layout (location = 1) in vec3 oNormal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 oTangent;
layout (location = 4) in vec3 oBitangent;

out VSOUT
{
    vec3 wVertex; // world space vertex position
    vec2 texCoord;
    vec3 wNormal;
} vsOut;

void main()
{
    vec3 oPos = oVertex;

    // 2 waves
    float wave1 = sin(oPos.x * 1.5 + _time.x * 1.2);
    float wave2 = sin(oPos.z * 2.0 + _time.x * 1.8);
    oPos.y += wave1 + wave2;
    vsOut.wVertex = vec3(_matrix_M * vec4(oPos, 1));


    vec3 tangent = vec3(1, cos(oPos.x * 1.5 + _time.x * 1.2), 0);
    vec3 bitangent = vec3(1, cos(oPos.z * 2.0 + _time.x * 1.8), 0);
    vsOut.wNormal = normalize(cross(tangent, bitangent));


    vsOut.texCoord = uv;

    gl_Position = _matrix_MVP * vec4(oPos, 1);
}
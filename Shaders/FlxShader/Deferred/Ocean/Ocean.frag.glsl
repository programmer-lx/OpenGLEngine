layout (location = 0) out vec4 gPositionDepth;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpecular;

in VSOUT
{
    vec3 wVertex;
    vec2 texCoord;
    vec3 wNormal;
} fsIn;

uniform Material material;

void main()
{
    // RGB: world space position
    gPositionDepth.rgb = fsIn.wVertex;
    // A: linear real depth (from near plane to fragment's view position)
    gPositionDepth.a = linearDepth(gl_FragCoord.z);

    // RG: world space normal's xy (tangent and bitangent)
    vec3 wNormal = normalize(fsIn.wNormal);
    wNormal.xy *= material.bumpScale;
    wNormal.z = sqrt(1.0f - max(0, dot(wNormal.xy, wNormal.xy)));
    gNormal.rg = wNormal.xy;
    // BA: view space normal's xy (tangent and bitangent)
    gNormal.ba = normalize(mat3(_matrix_V) * wNormal).xy;

    gAlbedoSpecular.rgb = texture(material.diffuseTex, fsIn.texCoord).rgb;
    gAlbedoSpecular.a = texture(material.specularTex, fsIn.texCoord).r;
}
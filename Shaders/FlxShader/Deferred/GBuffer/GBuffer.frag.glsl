layout (location = 0) out vec4 gPositionDepth;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpecular;

in VSOUT
{
    vec3 wVertex;
    vec2 texCoord;

} fsIn;

flat in mat3x3 T2W;

uniform Material material;

void main()
{
    // RGB: world space position
    gPositionDepth.rgb = fsIn.wVertex;
    // A: linear real depth (from near plane to fragment's view position)
    gPositionDepth.a = linearDepth(gl_FragCoord.z);

    // RG: world space normal's xy (tangent and bitangent)
    vec3 wNormal = normalize(T2W * decodeNormalTexture(material.normalTex, fsIn.texCoord));
    gNormal.rgb = wNormal.xyz;

    gAlbedoSpecular.rgb = texture(material.diffuseTex, fsIn.texCoord).rgb;
    gAlbedoSpecular.a = texture(material.specularTex, fsIn.texCoord).r;
}
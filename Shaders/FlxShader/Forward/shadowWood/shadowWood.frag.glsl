uniform Material material;
uniform vec4 tex_ST;
uniform Light dirLight;

out vec4 outColor;

#define TRANSFORM_TEX(coord, st) vec2(coord * st.xy + st.zw)

in VSOUT
{
    vec2 texCoord;
    vec3 wNormal;
    vec3 wVertex;
} fsin;

void main()
{
    outColor = vec4(calcDirLight_Lambert_BlinnPhong(material, TRANSFORM_TEX(fsin.texCoord, tex_ST), dirLight, fsin.wVertex, fsin.wNormal), 1);
}
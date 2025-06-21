
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform Material material;
uniform Light dirLight;
uniform sampler2D shadowMap;

vec4 tex_ST = vec4(1, 1, 0, 0);

float ShadowCalculation(vec4 fragPosLightSpace)
{
    float bias = 0.05;

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1)
        return 0;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += projCoords.z - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}

void main()
{
    DirLight light = _dirLights[_shadowDirLightIndex];

    vec3 diffuseColor = texture(material.diffuseTex, TRANSFORM_TEX(fs_in.TexCoords, tex_ST)).rgb * max(0, dot(normalize(fs_in.Normal), -light.direction.xyz));
    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    vec3 lighting = (dirLight.ambientColor * vec3(0.3f) + diffuseColor * (1.0 - shadow) * light.color.rgb) * diffuseColor;

    FragColor = vec4(lighting, 1.0);
}
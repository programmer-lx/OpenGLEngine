
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform Material material;
uniform samplerCube cubeShadowMap;

uniform float farPlane;

float ShadowCalculation(vec3 fragPos)
{
    vec3 lightToFrag = fragPos - _pointLights[_shadowPointLightIndex].position.xyz;
    float closestDepth = texture(cubeShadowMap, lightToFrag).r;
    closestDepth *= farPlane;
    float currentDepth = length(lightToFrag);
    float bias = 0.05;
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    vec3 color = texture(material.diffuseTex, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    // Ambient
    vec3 ambient = 0.1 * color;
    // Diffuse
    vec3 lightDir = normalize(_pointLights[_shadowPointLightIndex].position.xyz - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * _pointLights[0].color.rgb;
    // Specular
    vec3 viewDir = normalize(_cameraPos.xyz - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * _pointLights[0].color.rgb;
    // shadow
    float shadow = ShadowCalculation(fs_in.FragPos);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    FragColor = vec4(lighting, 1.0f);
}
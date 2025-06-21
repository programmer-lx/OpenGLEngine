out vec4 outColor;

in VSOUT
{
    vec2 texCoord;
} fsIn;

uniform samplerCube cubeShadowMap;
uniform float farPlane;

uniform sampler2D shadowMap;
uniform mat4x4 _dirLightSpaceMatrix_VP;

uniform bool enableShadowMap;
uniform bool enableSSAO;
uniform bool enableFog;

vec3 sampleEnvironmentColor(samplerCube skybox, vec3 dir)
{
    vec3 result = vec3(0);
    for (int i = 0; i < _ssaoKernelCount.x; ++i)
    {
        vec3 newDir = dir + _ssaoKernel[i].xyz;
        result += texture(skybox, newDir).rgb;
    }
    return result / _ssaoKernelCount.x;
}

float shadowCalculation_Cube(vec3 fragWorldPosition)
{
//    vec3 lightToFrag = fragWorldPosition - _pointLights[_shadowPointLightIndex].position.xyz;
//    float closestDepth = texture(cubeShadowMap, lightToFrag).r;
//    closestDepth *= farPlane;
//    float currentDepth = length(lightToFrag);
//    float bias = 0.05;
//    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
//
//    return shadow;

    vec3 fragToLight = fragWorldPosition - _pointLights[_shadowPointLightIndex].position.xyz;
    float currentDepth = length(fragToLight);

    // PCF
    float bias = 0.05;
    float shadow = 0.0;
    int samples = 20; // 采样点数量
    float diskRadius = 0.05; // 控制阴影模糊程度

    // 随机单位球方向，用于采样 cube map
    for(int i = 0; i < samples; ++i)
    {
        // 球面偏移方向
        vec3 offset = normalize(vec3(
        sin(float(i) * 12.9898),
        cos(float(i) * 78.233),
        sin(float(i) * 37.719)
        ));
        offset *= diskRadius;

        float sampleDepth = texture(cubeShadowMap, fragToLight + offset).r;
        sampleDepth *= farPlane;

        if(currentDepth - bias > sampleDepth)
            shadow += 1.0;
    }

    shadow /= float(samples);

    return shadow;
}

float shadowCalculation_DirLight(vec3 fragWorldPosition)
{
    float bias = 0.05;

    vec4 fragPosLightSpace = _dirLightSpaceMatrix_VP * vec4(fragWorldPosition, 1);

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0)
    {
        return 0.0;
    }

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

vec3 calcDirLight(DirLight light, vec3 albedo, float specular, vec3 wNormal, vec3 viewDir)
{
    vec3 lightDir = -light.direction.xyz;
    vec3 halfAngle = normalize(lightDir + viewDir);

    vec3 reflectDir = reflect(-viewDir, wNormal);
    albedo += sampleEnvironmentColor(_skyboxTex, reflectDir);

    // diffuse
    vec3 diffuseColor = light.color.rgb * max(0, dot(wNormal, -light.direction.xyz)) * albedo;

    // specular
    vec3 specularColor = light.color.rgb * pow(max(0, dot(wNormal, halfAngle)), 64) * specular;

    return diffuseColor + specularColor;
}

vec3 calcPointLight(PointLight light, vec3 albedo, float specular, vec3 wNormal, vec3 wVertex, vec3 viewDir)
{
    vec3 lightDir = light.position.xyz - wVertex;
    float dis = distance(light.position.xyz, wVertex);
    vec3 halfAngle = normalize(viewDir + lightDir);

    // diffuse
    vec3 diffuseColor = light.color.rgb * max(0, dot(wNormal, lightDir)) * albedo;

    // specular
    vec3 specularColor = light.color.rgb * pow(max(0, dot(wNormal, halfAngle)), 64) * specular;

    // attenuation
    float attenuation = 1.0f / (light.kc + light.kl * dis + light.kq * pow(dis, 2));

    return (diffuseColor + specularColor) * attenuation;
}

vec3 calcSpotLight(SpotLight light, vec3 albedo, float specular, vec3 wNormal, vec3 wVertex, vec3 viewDir)
{
    vec3 lightDir = light.position.xyz - wVertex;
    float dis = distance(light.position.xyz, wVertex);
    vec3 halfAngle = normalize(viewDir + lightDir);

    // diffuse
    vec3 diffuseColor = light.color.rgb * max(0, dot(wNormal, lightDir)) * albedo;

    // specular
    vec3 specularColor = light.color.rgb * pow(max(0, dot(wNormal, halfAngle)), 64) * specular;

    // attenuation
    float attenuation = 1.0f / (light.kc + light.kl * dis + light.kq * pow(dis, 2));

    // cut-off angle
    vec3 lightToVertex = normalize(wVertex - light.position.xyz);
    float curAngle = dot(lightToVertex, light.direction.xyz);
    float intensity = clamp((curAngle - light.cosOuterCone) / (light.cosInnerCone - light.cosOuterCone), 0, 1);

    return (diffuseColor + specularColor) * attenuation * intensity;
}

void main()
{
    vec4 albedoTexColor = texture(_gAlbedoSpecular, fsIn.texCoord);
    vec3 albedo = albedoTexColor.rgb;
    float specular = albedoTexColor.a;

    vec4 gNormalColor = texture(_gNormal, fsIn.texCoord);
    vec3 wNormal = texture(_gNormal, fsIn.texCoord).rgb;
    vec3 wVertex = texture(_gPositionDepth, fsIn.texCoord).rgb;

    vec3 viewDir = normalize(_cameraPos.xyz - wVertex);

    vec3 color = vec3(0);

    // AO
//#if ENABLE_SSAO
    float ao = 1.0f;
//    if (enableSSAO)
        ao = texture(_ssaoTex, fsIn.texCoord).r;
//#else
//#endif

    // dirLights
    for (int i = 0; i < _dirLightCount; ++i)
    {
        color += calcDirLight(_dirLights[i], albedo, specular, wNormal, viewDir);
    }

    // pointLights
    for (int i = 0; i < _pointLightCount; ++i)
    {
        color += calcPointLight(_pointLights[i], albedo, specular, wNormal, wVertex, viewDir);
    }

    // spotLights
    for (int i = 0; i < _spotLightCount; ++i)
    {
        color += calcSpotLight(_spotLights[i], albedo, specular, wNormal, wVertex, viewDir);
    }

//#if ENABLE_SHADOW_MAP
    float shadow = 0;
//    if (enableShadowMap)
//        shadow = shadowCalculation_Cube(wVertex);

    color *= (1.0 - shadow);
//#endif


    outColor = vec4(AMBIENT_LIGHT * albedo * ao + color + texture(_gEmission, fsIn.texCoord).rgb, 1);


//#if ENABLE_FOG
//    if (enableFog)
//    {
        float fogFactor = exp(-_fogDensity.x * distance(_cameraPos.xyz, wVertex.xyz));
        fogFactor = clamp(fogFactor, 0, 1);

        outColor = vec4(mix(_fogColor.rgb * _dirLights[0].color.rgb, outColor.rgb, fogFactor), 1);
//    }
//#endif
}
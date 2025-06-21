out vec4 outColor;

in VSOUT
{
    vec3 wVertex;
    vec2 texCoord;
} fsIn;

flat in vec3 wNormal;

uniform vec3 colorA;
uniform vec3 colorB;

void main()
{
    vec3 normal = normalize(wNormal);
    vec3 lightDir = -_dirLights[0].direction.xyz;
    float lightLerp = max(dot(normal, lightDir), 0.0);

    vec3 color = mix(colorB, colorA, lightLerp);

    vec3 viewDir = normalize(_cameraPos.xyz - fsIn.wVertex);
    vec3 halfAngle = normalize(viewDir + lightDir);

    // diffuse
    vec3 diffuseColor = color * _dirLights[0].color.rgb;

    // specular
    vec3 specularColor = color * 1.1 * _dirLights[0].color.rgb * pow(max(0, dot(normal, halfAngle)), 8);

    outColor = vec4(AMBIENT_LIGHT + diffuseColor + specularColor, 1.0);


#if ENABLE_FOG
    float fogFactor = exp(-_fogDensity.x * distance(_cameraPos.xyz, fsIn.wVertex.xyz));
    fogFactor = clamp(fogFactor, 0, 1);

    outColor = vec4(mix(_fogColor.rgb * _dirLights[0].color.rgb, outColor.rgb, fogFactor), 1);
#endif
}
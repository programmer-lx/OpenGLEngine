out float outColor;

in VSOUT
{
    vec2 texCoord;
} fsIn;

const float radius = 0.5f;
const float bias = 0.025f;
const float ssaoMaxDistance = 300.0;

void main()
{
    vec3 wVertex = texture(_gPositionDepth, fsIn.texCoord).rgb;
    vec4 gNormalTexColor = texture(_gNormal, fsIn.texCoord);
    vec4 gAlbedoTexColor = texture(_gAlbedoSpecular, fsIn.texCoord);
    vec2 texSize = textureSize(_ssaoNoiseTex, 0);

    vec3 wNormal = gNormalTexColor.rgb;

    vec3 vVertex = vec3(_matrix_V * vec4(wVertex, 1)); // view space vertex position
    vec3 vNormal = mat3x3(_matrix_WorldToViewNormal) * wNormal;
    vec3 vec = normalize(texture(_ssaoNoiseTex, fsIn.texCoord / texSize).xyz);
    vec3 vTangent = normalize(vec - vNormal * dot(vec, vNormal));
    vec3 vBitangent = normalize(cross(vNormal, vTangent));
    vTangent = normalize(cross(vNormal, vBitangent));

    mat3x3 vTBN = mat3x3(vTangent, vBitangent, vNormal);

    float occlusion = 0.0f;
    for (int i = 0; i < _ssaoKernelCount.x; ++i)
    {
        // view space sample point
        vec3 viewSamplePoint = vVertex + vTBN * _ssaoKernel[i].xyz * radius;

        // calculate sample point's NDC position
        vec4 ndcSamplePoint = vec4(viewSamplePoint, 1.0f);
        ndcSamplePoint = _matrix_P * ndcSamplePoint;
        // perspective divide
        ndcSamplePoint.xy /= ndcSamplePoint.w;
        // map [-1, 1] to [0, 1]
        ndcSamplePoint.xy = ndcSamplePoint.xy * 0.5f + 0.5f;

        // campare real depth and the depth that cache in texture
        // 因为在观察空间中，Z轴的负方向才是摄像机的正方向，所以 viewSamplePoint的Z轴是负的
        // 而裁剪空间又采用左手系，深度越深Z轴越大，与观察空间相反，所以这里要取负
        float texDepth = -texture(_gPositionDepth, ndcSamplePoint.xy).a;

        float rangeCheck = smoothstep(0.0f, 1.0f, radius / abs(vVertex.z - texDepth));
//        occlusion += (texDepth - bias >= viewSamplePoint.z ? 1.0f : 0.0f);
        occlusion += step(viewSamplePoint.z, texDepth - bias) * rangeCheck;
    }

    occlusion = 1.0f - (occlusion / _ssaoKernelCount.x);

    occlusion = mix(occlusion, 1, distance(wVertex, _cameraPos.xyz) / ssaoMaxDistance);

    outColor = occlusion;
}
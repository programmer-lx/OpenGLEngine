// from NDC depth to real depth (from camera's near plane to framgent)
float linearDepth(float nonLinearDepth)
{
    float ndcZ = nonLinearDepth * 2.0f - 1.0f;

//  return (2.0f *      near *       far) / (      far +      near - ndcZ * (      far -      near));
    return (2.0f * _camera.x * _camera.y) / (_camera.y + _camera.x - ndcZ * (_camera.y - _camera.x));
}

// from NDC depth to real depth, and map [near, far] to [0, 1]
float linear01Depth(float nonLinearDepth)
{
    //                                     near               far - near
    return (linearDepth(nonLinearDepth) - _camera.x) / (_camera.y - _camera.x);
}

vec3 decodeNormalTexture(sampler2D normalMap, vec2 uv)
{
    vec3 normal = texture(normalMap, uv).rgb;
    return normalize(normal * 2.0 - 1.0);
}

float rec709Brightness(vec3 color)
{
    return dot(color, vec3(0.2126, 0.7152, 0.0722));
}
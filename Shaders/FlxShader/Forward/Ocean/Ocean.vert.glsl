layout (location = 0) in vec3 oVertex;
layout (location = 1) in vec3 oNormal;
layout (location = 2) in vec2 uv;

out VSOUT
{
    vec3 wVertex;
    vec2 texCoord;
} vsOut;

flat out vec3 wNormal;

// 波浪参数
uniform vec4 waveFrequency; // 控制波长（越大波越密）
uniform vec4 waveAmplitude; // 控制波高
uniform vec4 waveSpeed;     // 控制波动速度

void main()
{
    vec3 oPos = oVertex; // 原始顶点位置



    // 多波干涉（两个方向正弦波）
    float waveA = waveAmplitude.x * sin(oPos.x * waveFrequency.x + _time.x * waveSpeed.x);
    float waveB = waveAmplitude.y * sin(oPos.z * waveFrequency.y + _time.x * waveSpeed.y);

    // 添加第三个波：斜向波 + 相位偏移
    float waveC = 1.5 * sin((oPos.x + oPos.z) * 2.0 + _time.x * 1.2 + 1.5); // 相位 +1.5

    // 合并波形
    oPos.y += waveA + waveB + waveC;

    float noise = noise1(oPos.xz * 0.2 + _time.x * 0.1);
    oPos.y += noise * 1.5;

    // 输出位置
    vsOut.wVertex = vec3(_matrix_M * vec4(oPos, 1.0));


    // 计算每个波在 x 和 z 上的偏导数
    float dWaveA_dx = waveAmplitude.x * waveFrequency.x * cos(oVertex.x * waveFrequency.x + _time.x * waveSpeed.x);
    float dWaveB_dz = waveAmplitude.y * waveFrequency.y * cos(oVertex.z * waveFrequency.y + _time.x * waveSpeed.y);
    float dWaveC_dx = 1.5 * 2.0 * cos((oVertex.x + oVertex.z) * 2.0 + _time.x * 1.2 + 1.5);
    float dWaveC_dz = dWaveC_dx; // 同时对 x 和 z 有影响

    // 组合偏导
    float dYdx = dWaveA_dx + dWaveC_dx;
    float dYdz = dWaveB_dz + dWaveC_dz;

    // 构造切线和副切线
    vec3 tangent   = normalize(vec3(1.0, dYdx, 0.0)); // dx 方向梯度
    vec3 bitangent = normalize(vec3(0.0, dYdz, 1.0)); // dz 方向梯度

    vec3 normal = normalize(cross(tangent, bitangent));

    // 转换到世界空间
    wNormal = normalize(_matrix_ObjectToWorldNormal * normal);


    vsOut.texCoord = uv;

    gl_Position = _matrix_MVP * vec4(oPos, 1);
}
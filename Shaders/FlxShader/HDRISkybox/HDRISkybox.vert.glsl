

layout (location = 0) in vec3 aPos;

out vec3 texCoord;
out vec3 oPos;

uniform mat4x4 skyboxMatrixVP;

void main()
{
    gl_Position = vec4(skyboxMatrixVP * vec4(aPos, 1)).xyww; // let z/w = 1.0f
    texCoord = aPos;
    oPos = aPos;
}
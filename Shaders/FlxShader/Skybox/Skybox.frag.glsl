

out vec4 outColor;

in vec3 texCoord;

uniform samplerCube skybox;
uniform vec3 skyboxColorMultiply;

void main()
{
    outColor = texture(skybox, texCoord) * vec4(skyboxColorMultiply, 1);
}
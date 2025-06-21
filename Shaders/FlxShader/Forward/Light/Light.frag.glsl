out vec4 outColor;

void main()
{
    outColor = vec4(sin(_time.x * 5), 1, 1, 1);
}
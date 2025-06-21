layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out; // 18: six triangle face

uniform mat4x4 lightSpaceMatrix_VP[6];

out GSOUT
{
    vec3 wVertex;
} gsout;

void main()
{
    // project every model's triangle face to the cube's 6 face: right, left, up, down, near, far

    for (int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // tell the face ID to OpenGL, 0: right, 1: left, ......
        for (int i = 0; i < 3; ++i)
        {
            gsout.wVertex = gl_in[i].gl_Position.xyz; // pass the world space position to the fragment shader
            gl_Position = lightSpaceMatrix_VP[face] * vec4(gsout.wVertex, 1); // transform to clip space
            // emit vertex
            EmitVertex();
        }
        EndPrimitive();
    }
}
[SHDR]
[Vertex]
#version 330
in vec3 a_Position;
in vec3 a_Normal;

uniform mat4 u_MVP;

out vec3 o_Color;

void main()
{
    gl_PointSize = 3.0;
    gl_Position = u_MVP * vec4(a_Position,1.0);
    o_Color = a_Normal;
}
[/Vertex]

[Fragment]
#version 330
in vec3 o_Color;
out vec4 Out0;

void main()
{
    Out0 = vec4(o_Color,1.0);
}
[/Fragment]
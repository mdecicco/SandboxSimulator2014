[SHDR]
[Vertex]
#version 330
in vec3 a_Position;
in vec3 a_Normal;
out vec3 o_Color;
void main()
{
    gl_Position = vec4(a_Position * vec3(0.1,0.1,0.1),1.0);
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
[SHDR]
[Vertex]
#version 330
in vec3 a_Position;
in vec3 a_Normal;
in vec2 a_TexCoord;

uniform mat4 u_MVP;

out vec3 o_Normal;
out vec2 o_TexC;

void main()
{
    gl_PointSize = 10;
    gl_Position = u_MVP * vec4(a_Position,1.0);
    
    o_Normal = a_Normal;
    o_TexC   = a_TexCoord;
}
[/Vertex]

[Fragment]
#version 330
in vec3 o_Normal;
in vec2 o_TexC;

uniform sampler2D u_Texture0;

out vec4 Out0;

void main()
{
    Out0 = texture(u_Texture0,o_TexC) * 0.5;
    Out0.xyz += o_Normal * 0.5;
}
[/Fragment]
[SHDR]
[Vertex]
#version 330
in vec3 a_Position;
in vec2 a_TexCoord;

uniform mat4 u_MVP;

out vec2 o_TexC;

void main()
{
    gl_Position = u_MVP * vec4(a_Position,1.0);
    o_TexC   = a_TexCoord;
}
[/Vertex]

[Fragment]
#version 330
in vec2 o_TexC;

uniform sampler2D u_Texture0;

out vec4 Out0;

void main() {
    Out0 = vec4(0,0,0,1);//texture(u_Texture0, o_TexC);
}
[/Fragment]

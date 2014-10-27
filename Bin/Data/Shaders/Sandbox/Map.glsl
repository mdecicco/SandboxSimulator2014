[SHDR]
[Vertex]
#version 330
in vec3 a_Position;
in vec3 a_Normal;
in vec2 a_TexCoord;

uniform mat4 u_MVP;
uniform mat4 u_Normal;

out vec3 o_Normal;
out vec2 o_TexC;

void main()
{
    gl_PointSize = 10;
    gl_Position = u_MVP * vec4(a_Position,1.0);
    
    o_Normal = (u_Normal * vec4(normalize(a_Normal),1.0)).xyz;
    o_TexC   = a_TexCoord;
}
[/Vertex]

[Fragment]
#version 330
in vec3 o_Normal;
in vec2 o_TexC;

uniform sampler2D u_Texture0;

uniform vec3 u_SunPosition;

out vec4 Out0;

void main()
{
    vec3 Dir = normalize(u_SunPosition);
    vec4 Diff = texture(u_Texture0,o_TexC);
    Out0 =  (Diff * 0.3) + (Diff * dot(o_Normal,Dir));
}
[/Fragment]
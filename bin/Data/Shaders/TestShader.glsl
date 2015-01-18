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
    gl_Position = u_MVP * vec4(a_Position,1.0);

    o_Normal = a_Normal;
    o_TexC   = a_TexCoord;
}
[/Vertex]

[Fragment]
#version 330
in vec3 o_Normal;
in vec2 o_TexC;

uniform vec3 u_SunPosition;
uniform float u_TimeOfDay;
uniform sampler2D u_Texture0;

out vec4 Out0;

void main()
{
    vec4 Albedo = texture(u_Texture0,o_TexC);
    if(Albedo.a == 0.0) discard;
    if(Albedo.a >= 0.95)
    {
        if(u_TimeOfDay <= 0.75 && u_TimeOfDay >= 0.25)
        {
            vec3 Dir = normalize(u_SunPosition);
            Out0 = (Albedo * dot(o_Normal,Dir));
        }
        else
        {
            vec3 Dir = -normalize(u_SunPosition);
            Out0 = (Albedo * dot(o_Normal,Dir) * 0.2);
        }
        Out0.a = Albedo.a;
    }
    else Out0 = Albedo;
    //Out0 = vec4(o_Normal,1);
}
[/Fragment]

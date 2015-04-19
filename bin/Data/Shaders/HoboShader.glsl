[SHDR]
[Vertex]
#version 330
in vec3 a_Position;
in vec3 a_Normal;
in vec2 a_TexCoord;

uniform mat4 u_MVP;
uniform mat4 u_Model;

out vec3 o_Normal;
out vec2 o_TexC;

void main()
{
    gl_Position = u_MVP * vec4(a_Position,1.0);

    o_Normal = inverse(transpose(mat3(u_Model))) * a_Normal;
    o_TexC   = vec2(a_TexCoord.x,1.0 - a_TexCoord.y);
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
    if(Albedo.a < 0.5) discard;
    if(Albedo.a >= 0.95)
    {
        if(u_TimeOfDay <= 0.75 && u_TimeOfDay >= 0.25)
        {
            vec3 Dir = normalize(u_SunPosition);
            float Factor = dot(o_Normal,Dir);
            Factor = max(Factor, 0.1);
            Out0 = Albedo * Factor;
        }
        else
        {
            Out0 = Albedo * 0.1;
        }
        Out0.a = Albedo.a;
    }
    else Out0 = Albedo;
    //Out0 = vec4(o_Normal,1);
}
[/Fragment]

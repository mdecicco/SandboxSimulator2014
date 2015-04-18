[SHDR]
[Vertex]
#version 330
in vec4 a_Position;
in vec3 a_Normal;
in vec2 a_TexCoord;

uniform mat4 u_MVP;
uniform mat4 u_ModelView;
uniform mat4 u_Model;
uniform vec2 u_FOV;
uniform vec2 u_Resolution;
uniform vec3 u_CameraPosition;

out vec3 o_Normal;

float WorldToPixels(float Distance,float DesiredSize)
{
    float h = 2 * Distance * tan((u_FOV.x * 0.01745329251) / 2);
    float r = DesiredSize / h;
    return u_Resolution.x * r;
}

void main()
{
    gl_Position = u_MVP * a_Position;

    mat4 normalMat = inverse(transpose(u_Model));
    o_Normal = mat3(normalMat) * a_Normal;

    vec4 wPos = u_Model * a_Position;
    float d = length(u_CameraPosition - wPos.xyz);
    float size = WorldToPixels(d,1.0);

    gl_PointSize = size;
}
[/Vertex]

[Fragment]
#version 330
in vec3 o_Normal;
in float o_SpriteFactor;

uniform vec3 u_SunPosition;
uniform float u_TimeOfDay;
uniform sampler2D u_Texture0;
uniform vec2 u_Resolution;

out vec4 Out0;

void main()
{
    vec2 texCoord = gl_PointCoord;
    vec4 Albedo = texture(u_Texture0,texCoord);
    vec4 Ambient = Albedo * 0.2;
    if(Albedo.a < 0.5) discard;
    if(Albedo.a >= 0.95)
    {
        vec3 Dir = normalize(u_SunPosition);
        float Factor = dot(o_Normal,Dir);
        Factor = max(Factor, 0.1);
        Out0 = (Albedo * Factor) + Ambient;
        Out0.a = Albedo.a;
    }
    else Out0 = Albedo;
    //Out0 = vec4(o_Normal,1);
}
[/Fragment]

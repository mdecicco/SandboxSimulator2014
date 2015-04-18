[SHDR]
[Vertex]
#version 330
in vec3 a_Position;
in vec3 a_Normal;
in vec2 a_TexCoord;

uniform mat4 u_MVP;
uniform mat4 u_ModelView;
uniform mat4 u_Model;
uniform vec3 u_CameraPosition;
uniform vec2 u_FOV;
uniform float u_NearPlane;
uniform float u_FarPlane;
uniform vec2 u_Resolution;

out vec3 o_Normal;

float PlaneDimensions(float size)
{
    float h = 2*u_NearPlane*tan(u_FOV.y/2);
    float nearHeight = u_Resolution.y * h;
    float r = size / nearHeight;
    return u_Resolution.y * r;
}

void main()
{
    gl_Position = u_MVP * vec4(a_Position,1.0);

    mat4 normalMat = inverse(transpose(u_Model));
    o_Normal = mat3(normalMat) * a_Normal;

    float size = PlaneDimensions(0.01f);
    float distance = (u_ModelView * vec4(a_Position, 1.0)).z;
    gl_PointSize = size / distance;
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

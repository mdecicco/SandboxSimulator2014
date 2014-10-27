[SHDR]
[Vertex]
#version 330
in vec3  a_Position;
in vec3  a_Normal  ;
in vec3  a_Tangent ;
in vec2  a_TexCoord;
in ivec4 a_BoneIDs ;
in vec4  a_Weights ;

uniform mat4 u_MVP;
uniform mat4 u_Normal;
uniform mat4 u_BoneMatrix[50];

out vec3 o_Normal;
out vec3 o_Tangent;
out vec2 o_TexCoord;

void main()
{
    gl_PointSize = 3.0;
    vec4 Pt = vec4(a_Position,1.0);
    if(a_BoneIDs.x != -1)
    {
        Pt = (Pt * u_BoneMatrix[a_BoneIDs.x]) * a_Weights.x;
        if(a_BoneIDs.y != -1)
        {
            Pt = (Pt * u_BoneMatrix[a_BoneIDs.y]) * a_Weights.y;
            if(a_BoneIDs.z != -1)
            {
                Pt = (Pt * u_BoneMatrix[a_BoneIDs.z]) * a_Weights.z;
                if(a_BoneIDs.w != -1)
                {
                    Pt = (Pt * u_BoneMatrix[a_BoneIDs.w]) * a_Weights.w;
                }
            }
        }
    }
    
    gl_Position = u_MVP * Pt;
    o_Normal = (u_Normal * vec4(normalize(a_Normal),1.0)).xyz;
    o_Tangent = a_Tangent;
    o_TexCoord = a_TexCoord;
}

[/Vertex]

[Fragment]
#version 330

in vec3 o_Normal;
in vec3 o_Tangent;
in vec2 o_TexCoord;

uniform vec3 u_SunPosition;
uniform vec4 u_SunLightColor;
uniform float u_SunIntensity;
uniform sampler2D u_Texture0;

out vec4 Out0;

void main()
{
    vec3 Dir = normalize(u_SunPosition);
    vec4 Diff = texture(u_Texture0,o_TexCoord);
    Out0 = (Diff * 0.3) + (Diff * dot(o_Normal,Dir) * (u_SunLightColor * u_SunIntensity));
}
[/Fragment]
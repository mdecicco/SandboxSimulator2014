[SHDR]
[Vertex]
#version 330
in  vec3 a_Position;
uniform mat4 u_MVP;
out vec3 o_Position;

void main()
{
    gl_PointSize = 10;
    gl_Position = u_MVP * vec4(a_Position,1.0);
    o_Position = a_Position;
}
[/Vertex]

[Fragment]
#version 330
in vec3 o_Position;

uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;
uniform vec3 u_SunPosition;

out vec4 Out0;

void main()
{
    vec3 V = normalize(o_Position);
    vec3 L = normalize(u_SunPosition);
    float vl = dot(V,L);
    vec4 Kc = texture(u_Texture0,vec2((L.y + 1.0) * 0.5,V.y));
    vec4 Kg = texture(u_Texture1,vec2((L.y + 1.0) * 0.5,max(vl,0.1)));
    Out0 = vec4(Kc.rgb + Kg.rgb * Kg.a * 0.5,Kc.a);
}
[/Fragment]
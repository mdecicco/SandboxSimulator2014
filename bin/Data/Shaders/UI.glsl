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

const float smoothing = 1.0/16.0;

void main() {
    vec4 distance = texture2D(u_Texture0, o_TexC);
    float distAlphaMask = distance.a;

    //if(distAlphaMask >= 0.0 && distAlphaMask <= 1) {
    //    float oFactor = 1.0;
    //    if(distAlphaMask <= 0.65) {
    //        oFactor = smoothstep(0.0, 0.65, distAlphaMask);
    //    } else {
    //        oFactor = smoothstep(1,0.65,distAlphaMask);
    //    }
    //    Out0 = mix(vec4(1,1,1,1), vec4(0,0,0,1), oFactor);
    //}

    //vec4 shadowTexel = texture2D(u_Texture0, o_TexC + vec2(-0.004,-0.004));
    //vec4 shadow = vec4(0,0,0,1) * smoothstep(0.5 - smoothing, 0.5 + smoothing, shadowTexel.a);

    float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, distAlphaMask);
    //if(distAlphaMask < 0.5) discard;
    Out0 = vec4(1,0,0,alpha);
}
[/Fragment]

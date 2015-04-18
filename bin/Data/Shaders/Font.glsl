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

uniform sampler2D u_Texture1;

out vec4 Out0;

const float smoothing = 1.0/24.0;

void main() {
    vec4 distance = texture(u_Texture1, o_TexC);
    float distAlphaMask = distance.a;

    //vec4 InnerColor = vec4(1,1,1,1);
    //vec4 OutlineColor = vec4(0,0,0,1);
    //Out0 = OutlineColor;
    //if(distAlphaMask >= 0.5 && distAlphaMask <= 1.0) {
    //    float oFactor = 1.0;
    //    if(distAlphaMask <= 0.75) {
    //        oFactor = smoothstep(0.5, 0.75, distAlphaMask);
    //    } else {
    //        oFactor = smoothstep(1.0,0.75,distAlphaMask);
    //    }
    //    Out0 = mix(OutlineColor, InnerColor, oFactor);
    //}

    vec4 shadowTexel = texture(u_Texture1, o_TexC + vec2(-0.004,-0.004));
    vec4 shadow = vec4(0,0,0,1) * smoothstep(0.5 - smoothing, 0.5 + smoothing, shadowTexel.a);
    Out0 = vec4(1,1,1,1);
    float alpha = smoothstep(0.5,0.55, distAlphaMask);
    //if(distAlphaMask <= 0.5) discard;
    Out0.a *= alpha;
    Out0 = mix(shadow, Out0, Out0.a);
}
[/Fragment]

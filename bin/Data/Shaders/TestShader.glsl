[SHDR]
[Vertex]
    #version 330
    in vec3 vp;
    uniform mat4 u_Model;
    uniform mat4 u_Projection;
    out vec3 NormalVert;
    void main () {
        NormalVert = vec3(0,0,1);
        gl_Position = u_Projection * u_Model * vec4(vp, 1.0);
    }
[/Vertex]
[Fragment]
    #version 330
    in vec3 NormalVert;
    out vec4 frag_colour;
    void main () {
        vec3 Albedo = vec3(0,0,1);
        frag_colour = vec4(Albedo,1.0);
    }
[/Fragment]

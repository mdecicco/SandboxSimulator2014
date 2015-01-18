[SHDR]
[Vertex]
#version 330
uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform float u_FarPlane;
float fInnerRadius = 3186.17;         // The inner (planetary) radius
float fOuterRadius = 3278.11;         // The outer (atmosphere) radius

in vec3 a_Position;

out vec3 oPosition;

void main(void)
{
    vec4 VtxPos = vec4(a_Position * u_FarPlane,1.0);
    gl_Position =  u_MVP * VtxPos;
    oPosition = (u_Model * vec4(normalize(a_Position) * fOuterRadius,1.0)).xyz;
}
[/Vertex]

[Fragment]
#version 330
uniform vec3  u_SunPosition;
uniform vec3  u_CameraPosition;
uniform float u_FarPlane;

float pi = 3.14159265;
vec3  waveLength = vec3(0.65,0.57,0.475);
vec3  v3InvWavelength = vec3(1.0f / pow(waveLength.r,4.0f),1.0f / pow(waveLength.g,4.0f),1.0f / pow(waveLength.b,4.0f));
float fOuterRadius = 3398.00f;
float fOuterRadius2 = fOuterRadius * fOuterRadius;
float fInnerRadius = 3286.00f;
float fInnerRadius2 = fInnerRadius * fInnerRadius;
float fkm = 0.000713832f;
float fkr = 0.00143956f;
float esun = 31.223f;
float fKrESun = fkr * esun;
float fKmESun = fkm * esun;
float fKr4PI = fkr*4*pi;
float fKm4PI = fkm*4*pi;
float fScale = 1.0 / (fOuterRadius - fInnerRadius);
float fScaleDepth = 0.832589f;
float fScaleOverScaleDepth = fScale / fScaleDepth;
float g = -0.995f;
float g2 = g*g;

int   nSamples = 5;
float fSamples = 5.0f;

in vec3 oPosition;

out vec4 Out0;

float scale(float fCos)
{
    float x = 1.0 - fCos;
    return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}
float czm_luminance(vec3 rgb)
{
    // Algorithm from Chapter 10 of Graphics Shaders.
    return dot(rgb,vec3(0.2125, 0.7154, 0.0721));
}
void main (void)
{
    vec3 v3CameraPos = u_CameraPosition;
    vec3 v3LightPos  = normalize(u_SunPosition);
    float fCameraHeight  = (v3CameraPos.y * 0.001) + fInnerRadius;
    float fCameraHeight2 = fCameraHeight* fCameraHeight;

    // Get the ray from the camera to the vertex, and its length (which is the far point of the ray passing through the atmosphere)
    vec3 v3Pos = oPosition;
    vec3 v3Ray = v3Pos - v3CameraPos;
    float fFar = length(v3Ray);
    v3Ray /= fFar;

    // Calculate the ray's starting position, then calculate its scattering offset
    vec3 v3Start = (v3CameraPos * 0.001) + vec3(0,fInnerRadius,0);
    float fHeight = length(v3Start);
    float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
    float fStartAngle = dot(v3Ray, v3Start) / fHeight;
    float fStartOffset = fDepth * scale(fStartAngle);

    // Initialize the scattering loop variables
    float fSampleLength = fFar / fSamples;
    float fScaledLength = fSampleLength * fScale;
    vec3 v3SampleRay = v3Ray * fSampleLength;
    vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

    // Now loop through the sample rays
    vec3 v3FrontColor = vec3(0.2,0.1,0.1);
    for(int i = 0;i < nSamples;i++)
    {
        float fHeight = length(v3SamplePoint);
        float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
        float fLightAngle = dot(v3LightPos, v3SamplePoint) / fHeight;
        float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
        float fScatter = (fStartOffset + fDepth*(scale(fLightAngle) - scale(fCameraAngle)));
        vec3 v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
        v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
        v3SamplePoint += v3SampleRay;
    }

    // Finally, scale the Mie and Rayleigh colors
    vec3 seccolor = v3FrontColor * fKmESun;
    vec3 color = v3FrontColor * (v3InvWavelength * fKrESun);
    vec3 v3Direction = v3CameraPos - v3Pos;

    float fCos = dot(v3LightPos,v3Direction) / length(v3Direction);
    float fMiePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos*fCos) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
    Out0 = vec4(color,1.0) + fMiePhase * vec4(seccolor,1.0);
    Out0.a = czm_luminance(Out0.rgb) * 2.0;
}
[/Fragment]

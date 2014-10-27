/*
 *  Shader.cpp
 *
 *  Created by Michael DeCicco on 7/7/14.
 *
 */

#include "Shader.h"

namespace Reality
{
    static Literal UniformNames[SU_COUNT] =
    {
        "u_Model",
        "u_Normal",
        "u_View",
        "u_Projection",
        "u_MVP",
        "u_BoneMatrix[0]",
        "u_BoneMatrix[1]",
        "u_BoneMatrix[2]",
        "u_BoneMatrix[3]",
        "u_BoneMatrix[4]",
        "u_BoneMatrix[5]",
        "u_BoneMatrix[6]",
        "u_BoneMatrix[7]",
        "u_BoneMatrix[8]",
        "u_BoneMatrix[9]",
        "u_BoneMatrix[10]",
        "u_BoneMatrix[11]",
        "u_BoneMatrix[12]",
        "u_BoneMatrix[13]",
        "u_BoneMatrix[14]",
        "u_BoneMatrix[15]",
        "u_BoneMatrix[16]",
        "u_BoneMatrix[17]",
        "u_BoneMatrix[18]",
        "u_BoneMatrix[19]",
        "u_BoneMatrix[20]",
        "u_BoneMatrix[21]",
        "u_BoneMatrix[22]",
        "u_BoneMatrix[23]",
        "u_BoneMatrix[24]",
        "u_BoneMatrix[25]",
        "u_BoneMatrix[26]",
        "u_BoneMatrix[27]",
        "u_BoneMatrix[28]",
        "u_BoneMatrix[29]",
        "u_BoneMatrix[30]",
        "u_BoneMatrix[31]",
        "u_BoneMatrix[32]",
        "u_BoneMatrix[33]",
        "u_BoneMatrix[34]",
        "u_BoneMatrix[35]",
        "u_BoneMatrix[36]",
        "u_BoneMatrix[37]",
        "u_BoneMatrix[38]",
        "u_BoneMatrix[39]",
        "u_BoneMatrix[40]",
        "u_BoneMatrix[41]",
        "u_BoneMatrix[42]",
        "u_BoneMatrix[43]",
        "u_BoneMatrix[44]",
        "u_BoneMatrix[45]",
        "u_BoneMatrix[46]",
        "u_BoneMatrix[47]",
        "u_BoneMatrix[48]",
        "u_BoneMatrix[49]",
        "u_BoneMatrix[50]",
        "u_Exposure",
        "u_NearPlane",
        "u_FarPlane",
        "u_TimeOfDay",
        "u_SunPosition",
        "u_Texture0",
        "u_Texture1",
        "u_Texture2",
        "u_Texture3",
        "u_Texture4",
        "u_Texture5",
        "u_Texture6",
        "u_Texture7",
        "u_Texture8",
    };
    
    Shader::Shader() : m_VS(0), m_PS(0), m_PID(0), m_ListID(-1)
    {
        for(i32 i = 0;i < SU_COUNT;i++) m_UniformLocs[i] = -1;
    }
    Shader::~Shader()
    {
        if(m_VS) { glDeleteShader(m_VS); m_VS = 0; }
        if(m_PS) { glDeleteShader(m_PS); m_PS = 0; }
        if(m_PID) { glDeleteProgram(m_PID); m_PID = 0; }
    }

    bool Shader::Reload()
    {
        GLuint ugh;
        glGenVertexArrays(1,&ugh);
        glBindVertexArray(ugh);
        
        m_VS = glCreateShader(GL_VERTEX_SHADER);
        m_PS = glCreateShader(GL_FRAGMENT_SHADER);
        
        SetPosition(0);
        char Hdr[6];
        Read(Hdr);
        if(Hdr[0] == '['
        && Hdr[1] == 'S'
        && Hdr[2] == 'H'
        && Hdr[3] == 'D'
        && Hdr[4] == 'R'
        && Hdr[5] == ']')
        {
            i32 Vtx0 = 0;
            i32 Vtx1 = 0;
            i32 Frg0 = 0;
            i32 Frg1 = 0;
            bool FoundVtx0 = false;
            bool FoundVtx1 = false;
            bool FoundFrg0 = false;
            bool FoundFrg1 = false;
            /* Acquire shader start/end indices */
            while(!AtEnd())
            {
                char c = GetC();
                if(c == '[')
                {
                    i32 Idx0 = GetPosition();
                    i32 Idx1 = 0;
                    bool FoundEnd = false;
                    while(!AtEnd())
                    {
                        c = GetC();
                        if(c == ']')
                        {
                            FoundEnd = true;
                            Idx1 = GetPosition() - 1;
                            break;
                        }
                    }
                    
                    SetPosition(Idx0);
                    if(Idx1 - Idx0 == 6 && !strncmp((CString)GetPtr(),"Vertex",6))
                    {
                        FoundVtx0 = true;
                        Vtx0 = Idx1 + 1;
                        SetPosition(Vtx0);
                        while(!AtEnd())
                        {
                            char c = GetC();
                            if(c == '[')
                            {
                                Idx0 = GetPosition();
                                Idx1 = 0;
                                bool FoundEnd = false;
                                while(!AtEnd())
                                {
                                    c = GetC();
                                    if(c == ']')
                                    {
                                        FoundEnd = true;
                                        Idx1 = GetPosition() - 1;
                                        break;
                                    }
                                }
                                
                                SetPosition(Idx0);
                                if(Idx1 - Idx0 == 7 && !strncmp((CString)GetPtr(),"/Vertex",7))
                                {
                                    FoundVtx1 = true;
                                    Vtx1 = Idx0 - 2;
                                    break;
                                }
                            }
                        }
                    }
                    else if(Idx1 - Idx0 == 8 && !strncmp((CString)GetPtr(),"Fragment",8))
                    {
                        FoundFrg0 = true;
                        Frg0 = Idx1 + 1;
                        SetPosition(Frg0);
                        while(!AtEnd())
                        {
                            char c = GetC();
                            if(c == '[')
                            {
                                Idx0 = GetPosition();
                                Idx1 = 0;
                                bool FoundEnd = false;
                                while(!AtEnd())
                                {
                                    c = GetC();
                                    if(c == ']')
                                    {
                                        FoundEnd = true;
                                        Idx1 = GetPosition() - 1;
                                        break;
                                    }
                                }
                                
                                SetPosition(Idx0);
                                if(Idx1 - Idx0 == 9 && !strncmp((CString)GetPtr(),"/Fragment",9))
                                {
                                    FoundFrg1 = true;
                                    Frg1 = Idx0 - 2;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            
            
            if(!FoundVtx0)
            {
                Log("Unable to load shader <%s>. Couldn't find vertex shader section header \"[Vertex]\".\n",m_Path.c_str());
                glDeleteShader(m_VS); m_VS = 0;
                glDeleteShader(m_PS); m_PS = 0;
                glBindVertexArray(0);
                glDeleteVertexArrays(1,&ugh);
                return false;
            }
            if(!FoundVtx1)
            {
                Log("Unable to load shader <%s>. (Expected \"[/Vertex]\")\n",m_Path.c_str());
                glDeleteShader(m_VS); m_VS = 0;
                glDeleteShader(m_PS); m_PS = 0;
                glBindVertexArray(0);
                glDeleteVertexArrays(1,&ugh);
                return false;
            }
            if(!FoundFrg0)
            {
                Log("Unable to load shader <%s>. Couldn't find fragment shader section header \"[Fragment]\".\n",m_Path.c_str());
                glDeleteShader(m_VS); m_VS = 0;
                glDeleteShader(m_PS); m_PS = 0;
                glBindVertexArray(0);
                glDeleteVertexArrays(1,&ugh);
                return false;
            }
            if(!FoundFrg1)
            {
                Log("Unable to load shader <%s>. (Expected \"[/Fragment]\")\n",m_Path.c_str());
                glDeleteShader(m_VS); m_VS = 0;
                glDeleteShader(m_PS); m_PS = 0;
                glBindVertexArray(0);
                glDeleteVertexArrays(1,&ugh);
                return false;
            }
            
            /* Compile sources */
            SetPosition(Vtx0);
            i32 VtxLen = Vtx1 - Vtx0;
            CString VtxPrg = (CString)GetPtr();
            glShaderSource(m_VS,1,&VtxPrg,&VtxLen);
            glCompileShader(m_VS);
            GLint Status;
            
            glGetShaderiv(m_VS,GL_COMPILE_STATUS,&Status);
            if(!Status)
            {
                GLint LogLen;
                glGetShaderiv(m_VS,GL_INFO_LOG_LENGTH,&LogLen);
                char iLog[LogLen];
                glGetShaderInfoLog(m_VS,LogLen,&LogLen,iLog);
            
                Log("Unable to compile vertex shader <%s>.\n",m_Path.c_str());
                Log("Info: %s\n",iLog);
                glDeleteShader(m_VS);
                glDeleteShader(m_PS);
                m_VS = m_PS = 0;
                glBindVertexArray(0);
                glDeleteVertexArrays(1,&ugh);
                return false;
            }
            
            SetPosition(Frg0);
            i32 FrgLen = Frg1 - Frg0;
            CString FrgPrg = (CString)GetPtr();
            glShaderSource(m_PS,1,&FrgPrg,&FrgLen);
            glCompileShader(m_PS);
            
            glGetShaderiv(m_PS,GL_COMPILE_STATUS,&Status);
            if(!Status)
            {
                GLint LogLen;
                glGetShaderiv(m_PS,GL_INFO_LOG_LENGTH,&LogLen);
                char iLog[LogLen];
                glGetShaderInfoLog(m_PS,LogLen,&LogLen,iLog);
            
                Log("Unable to compile fragment shader <%s>.\n",m_Path.c_str());
                Log("Info: %s\n",iLog);
                glDeleteShader(m_VS);
                glDeleteShader(m_PS);
                m_VS = m_PS = 0;
                glBindVertexArray(0);
                glDeleteVertexArrays(1,&ugh);
                return false;
            }
            
            /* Link program */
            m_PID = glCreateProgram();
            glAttachShader(m_PID,m_VS);
            glAttachShader(m_PID,m_PS);
            glBindFragDataLocation(m_PID,0,"Out0");
            glBindAttribLocation(m_PID,VA_POSITION,"a_Position");
            glBindAttribLocation(m_PID,VA_NORMAL  ,"a_Normal"  );
            glBindAttribLocation(m_PID,VA_TANGENT ,"a_Tangent" );
            glBindAttribLocation(m_PID,VA_TEXCOORD,"a_TexCoord");
            glBindAttribLocation(m_PID,VA_BONEIDX ,"a_BoneIDs");
            glBindAttribLocation(m_PID,VA_WEIGHT  ,"a_Weights");
            
            glLinkProgram(m_PID);
            glGetProgramiv(m_PID,GL_LINK_STATUS,&Status);
            if(!Status)
            {
                GLint LogLen;
                glGetProgramiv(m_PS,GL_INFO_LOG_LENGTH,&LogLen);
                char iLog[LogLen];
                glGetProgramInfoLog(m_PID,LogLen,&LogLen,iLog);
                Log("Unable to link shader <%s>.\n",m_Path.c_str());
                Log("Info: %s\n",iLog);
                glDeleteProgram(m_PID);
                glDeleteShader(m_VS);
                glDeleteShader(m_PS);
                m_PID = m_VS = m_PS = 0;
                glBindVertexArray(0);
                glDeleteVertexArrays(1,&ugh);
                return false;
            }
            
            glValidateProgram(m_PID);
            glGetProgramiv(m_PID,GL_VALIDATE_STATUS,&Status);
            if(!Status)
            {
                GLint LogLen;
                glGetProgramiv(m_PID,GL_INFO_LOG_LENGTH,&LogLen);
                char iLog[LogLen];
                glGetProgramInfoLog(m_PID,LogLen,&LogLen,iLog);
                Log("Unable to validate shader <%s>.\n",m_Path.c_str());
                Log("Info: %s\n",iLog);
                glDeleteProgram(m_PID);
                glDeleteShader(m_VS);
                glDeleteShader(m_PS);
                m_PID = m_VS = m_PS = 0;
                glBindVertexArray(0);
                glDeleteVertexArrays(1,&ugh);
                return false;
            }
        }
        else
        {
            glDeleteShader(m_VS); m_VS = 0;
            glDeleteShader(m_PS); m_PS = 0;
            glBindVertexArray(0);
            glDeleteVertexArrays(1,&ugh);
            Log("Unable to load shader. Invalid file <%s>. (File doesn't begin with [SHDR])\n",m_Path.c_str());
            return false;
        }
        
        for(i32 i = 0;i < SU_COUNT;i++) m_UniformLocs[i] = glGetUniformLocation(m_PID,UniformNames[i]);
        
        glBindVertexArray(0);
        glDeleteVertexArrays(1,&ugh);
        return true;
    }
    void Shader::Enable()
    {
        glUseProgram(m_PID);
    }
    void Shader::Disable()
    {
        glUseProgram(0);
    }
};
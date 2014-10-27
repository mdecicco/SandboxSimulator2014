/*
 *  ScriptFunctionality.cpp
 *
 *  Created by Michael DeCicco on 6/27/14.
 *
 */

#include "ScriptFunctionality.h"
#include "Engine.h"
#include "Lights.h"
#include "Event.h"

#include <string>
using namespace std;

namespace Reality
{
    void Print(const string& Text) { Engine::Log("%s",Text.c_str()); }
    Entity* EntityFactory() { return Engine::GetEngine()->CreateEntity(); }
    void Evt1(i32 EventID,void *Memory) { new(Memory) Event(EventID); }
    void Evt2(const Event& E,void *Memory) { new(Memory) Event(E); }
    void Evt3(void *Memory) { new(Memory) Event(-1); }
    void Evt4(void *Memory) { ((Event*)Memory)->~Event(); }
    void RegisterScriptFunctionality()
    {
        ScriptSystem* Sys = Engine::GetEngine()->GetScriptSystem();
        Sys->RegisterFunction("void Print(const string& in)",asFUNCTION(Print));
        
        /* Event class */
        Sys->RegisterClass("Event",sizeof(Event),asOBJ_VALUE | asOBJ_APP_CLASS_CDK);
        Sys->RegisterClassBehavior("Event",asBEHAVE_CONSTRUCT,"void f(int EventID)",asFUNCTION(Evt1),asCALL_CDECL_OBJLAST);
        Sys->RegisterClassBehavior("Event",asBEHAVE_CONSTRUCT,"void f(const Event& in)",asFUNCTION(Evt2),asCALL_CDECL_OBJLAST);
        Sys->RegisterClassBehavior("Event",asBEHAVE_CONSTRUCT,"void f()",asFUNCTION(Evt3),asCALL_CDECL_OBJLAST);
        Sys->RegisterClassBehavior("Event",asBEHAVE_DESTRUCT,"void f()",asFUNCTION(Evt4),asCALL_CDECL_OBJLAST);
        Sys->RegisterClassMethod("Event","int ID() const",asMETHOD(Event,ID));
        Sys->RegisterClassMethod("Event","void AddParami(int p)",asMETHOD(Event,AddParami));
        Sys->RegisterClassMethod("Event","void AddParamf(Scalar p)",asMETHOD(Event,AddParamf));
        Sys->RegisterClassMethod("Event","void AddParamd(double p)",asMETHOD(Event,AddParamd));
        Sys->RegisterClassMethod("Event","void AddParamv2(const Vector2& in)",asMETHOD(Event,AddParamv2));
        Sys->RegisterClassMethod("Event","void AddParamv3(const Vector3& in)",asMETHOD(Event,AddParamv3));
        Sys->RegisterClassMethod("Event","void AddParamv4(const Vector4& in)",asMETHOD(Event,AddParamv4));
        Sys->RegisterClassMethod("Event","void AddParamq(const Quaternion& in)",asMETHOD(Event,AddParamq));
        Sys->RegisterClassMethod("Event","void AddParamm(const Matrix4& in)",asMETHOD(Event,AddParamm));
        Sys->RegisterClassMethod("Event","void AddParams(const string& in)",asMETHOD(Event,AddParams));
        Sys->RegisterClassMethod("Event","int GetParami(int Idx) const",asMETHOD(Event,GetParami));
        Sys->RegisterClassMethod("Event","Scalar GetParamf(int Idx) const",asMETHOD(Event,GetParamf));
        Sys->RegisterClassMethod("Event","double GetParamd(int Idx) const",asMETHOD(Event,GetParamd));
        Sys->RegisterClassMethod("Event","Vector2 GetParamv2(int Idx) const",asMETHOD(Event,GetParamv2));
        Sys->RegisterClassMethod("Event","Vector3 GetParamv3(int Idx) const",asMETHOD(Event,GetParamv3));
        Sys->RegisterClassMethod("Event","Vector4 GetParamv4(int Idx) const",asMETHOD(Event,GetParamv4));
        Sys->RegisterClassMethod("Event","Quaternion GetParamq(int Idx) const",asMETHOD(Event,GetParamq));
        Sys->RegisterClassMethod("Event","Matrix4 GetParamm(int Idx) const",asMETHOD(Event,GetParamm));
        Sys->RegisterClassMethod("Event","string GetParams(int Idx) const",asMETHOD(Event,GetParams));
        
        
        /* Entity class */
        Sys->RegisterClass("Entity",sizeof(Entity),asOBJ_REF);
        Sys->RegisterClassBehavior("Entity",asBEHAVE_ADDREF,"void f()",asMETHOD(Entity,AddRef),asCALL_THISCALL);
        Sys->RegisterClassBehavior("Entity",asBEHAVE_RELEASE,"void f()",asMETHOD(Entity,Destroy),asCALL_THISCALL);
        Sys->RegisterClassBehavior("Entity",asBEHAVE_FACTORY,"Entity@ f()",asFUNCTION(EntityFactory),asCALL_CDECL);
        Sys->RegisterClassMethod("Entity","int GetID() const",asMETHOD(Entity,GetID));
        Sys->RegisterClassMethod("Entity","void AddScript(const string& in)",asMETHOD(Entity,AddScript));
        
        /* Renderable functions */
        Sys->RegisterEnum("PRIMITIVE_TYPE");
        Sys->RegisterEnumValue("PRIMITIVE_TYPE","POINTS",GL_POINTS);
        Sys->RegisterEnumValue("PRIMITIVE_TYPE","LINES",GL_LINES);
        Sys->RegisterEnumValue("PRIMITIVE_TYPE","LINE_LOOP",GL_LINE_LOOP);
        Sys->RegisterEnumValue("PRIMITIVE_TYPE","LINE_STRIP",GL_LINE_STRIP);
        Sys->RegisterEnumValue("PRIMITIVE_TYPE","TRIANGLES",GL_TRIANGLES);
        Sys->RegisterEnumValue("PRIMITIVE_TYPE","TRIANGLE_STRIP",GL_TRIANGLE_STRIP);
        Sys->RegisterEnumValue("PRIMITIVE_TYPE","TRIANGLE_FAN",GL_TRIANGLE_FAN);
        
        Sys->RegisterEnum("Glenum");
        Sys->RegisterEnumValue("Glenum","TEXTURE_MIN_FILTER",GL_TEXTURE_MIN_FILTER);
        Sys->RegisterEnumValue("Glenum","TEXTURE_MAG_FILTER",GL_TEXTURE_MAG_FILTER);
        Sys->RegisterEnumValue("Glenum","TEXTURE_WRAP_X",GL_TEXTURE_WRAP_S);
        Sys->RegisterEnumValue("Glenum","TEXTURE_WRAP_Y",GL_TEXTURE_WRAP_T);
        Sys->RegisterEnumValue("Glenum","NEAREST",GL_NEAREST);
        Sys->RegisterEnumValue("Glenum","LINEAR",GL_LINEAR);
        Sys->RegisterEnumValue("Glenum","NEAREST_MIPMAP_LINEAR",GL_NEAREST_MIPMAP_LINEAR);
        Sys->RegisterEnumValue("Glenum","NEAREST_MIPMAP_NEAREST",GL_NEAREST_MIPMAP_NEAREST);
        Sys->RegisterEnumValue("Glenum","LINEAR_MIPMAP_LINEAR",GL_LINEAR_MIPMAP_LINEAR);
        Sys->RegisterEnumValue("Glenum","LINEAR_MIPMAP_NEAREST",GL_LINEAR_MIPMAP_NEAREST);
        Sys->RegisterEnumValue("Glenum","CLAMP_TO_EDGE",GL_CLAMP_TO_EDGE);
        Sys->RegisterEnumValue("Glenum","CLAMP_TO_BORDER",GL_CLAMP_TO_BORDER);
        Sys->RegisterEnumValue("Glenum","MIRRORED_REPEAT",GL_MIRRORED_REPEAT);
        Sys->RegisterEnumValue("Glenum","REPEAT",GL_REPEAT);
        
        Sys->RegisterEnum("LIGHT_TYPE");
        Sys->RegisterEnumValue("LIGHT_TYPE","POINT_LIGHT",LightComponent::LT_POINT);
        Sys->RegisterEnumValue("LIGHT_TYPE","SPOT_LIGHT",LightComponent::LT_SPOT);
        Sys->RegisterEnumValue("LIGHT_TYPE","DIRECTIONAL_LIGHT",LightComponent::LT_DIRECTIONAL);
        
        Sys->RegisterClassMethod("Entity","void SetGUIFlag(bool Flag)",asMETHOD(Entity,SetGUIFlag));
        Sys->RegisterClassMethod("Entity","int ID() const",asMETHOD(Entity,GetID));
        Sys->RegisterClassMethod("Entity","void AddText(const string& in,const Vector3& in,int TextureID,const string& in)",asMETHOD(Entity,AddText));
        Sys->RegisterClassMethod("Entity","bool IsGUI() const",asMETHOD(Entity,IsGUI));
        Sys->RegisterClassMethod("Entity","void ClearVertices()",asMETHOD(Entity,ClearVertices));
        Sys->RegisterClassMethod("Entity","void AddVertex(const Vector3& in)",asMETHOD(Entity,AddVertex));
        Sys->RegisterClassMethod("Entity","void AddVertex(Scalar x,Scalar y,Scalar z)",asMETHOD(Entity,AddVertex_));
        Sys->RegisterClassMethod("Entity","void SetVertex(int Idx,const Vector3& in)",asMETHOD(Entity,SetVertex));
        Sys->RegisterClassMethod("Entity","Vector3 GetVertex(int Idx)",asMETHOD(Entity,GetVertex));
        Sys->RegisterClassMethod("Entity","void AddTexCoord(const Vector2& in)",asMETHOD(Entity,AddTexCoord));
        Sys->RegisterClassMethod("Entity","void AddTexCoord(Scalar x,Scalar y)",asMETHOD(Entity,AddTexCoord_));
        Sys->RegisterClassMethod("Entity","void SetTexCoord(int Idx,const Vector2& in)",asMETHOD(Entity,SetTexCoord));
        Sys->RegisterClassMethod("Entity","Vector2 GetTexCoord(int Idx)",asMETHOD(Entity,GetTexCoord));
        Sys->RegisterClassMethod("Entity","void AddNormal(const Vector3& in)",asMETHOD(Entity,AddNormal));
        Sys->RegisterClassMethod("Entity","void AddNormal(Scalar x,Scalar y,Scalar z)",asMETHOD(Entity,AddNormal_));
        Sys->RegisterClassMethod("Entity","void SetNormal(int Idx,const Vector3& in)",asMETHOD(Entity,SetNormal));
        Sys->RegisterClassMethod("Entity","Vector3 GetNormal(int Idx)",asMETHOD(Entity,GetNormal));
        Sys->RegisterClassMethod("Entity","void AddTangent(const Vector3& in)",asMETHOD(Entity,AddTangent));
        Sys->RegisterClassMethod("Entity","void AddTangent(Scalar x,Scalar y,Scalar z)",asMETHOD(Entity,AddTangent_));
        Sys->RegisterClassMethod("Entity","void SetTangent(int Idx,const Vector3& in)",asMETHOD(Entity,SetTangent));
        Sys->RegisterClassMethod("Entity","Vector3 GetTangent(int Idx)",asMETHOD(Entity,GetTangent));
        Sys->RegisterClassMethod("Entity","void AddWeight1(Scalar Weight)",asMETHOD(Entity,AddVertexWeight1));
        Sys->RegisterClassMethod("Entity","void AddWeight2(Scalar Weight0,Scalar Weight1)",asMETHOD(Entity,AddVertexWeight2));
        Sys->RegisterClassMethod("Entity","void AddWeight3(Scalar Weight0,Scalar Weight1,Scalar Weight2)",asMETHOD(Entity,AddVertexWeight3));
        Sys->RegisterClassMethod("Entity","void AddWeight4(Scalar Weight0,Scalar Weight1,Scalar Weight2,Scalar Weight3)",asMETHOD(Entity,AddVertexWeight4));
        Sys->RegisterClassMethod("Entity","void SetWeight1(int Idx,Scalar Weight)",asMETHOD(Entity,SetWeight0));
        Sys->RegisterClassMethod("Entity","void SetWeight2(int Idx,Scalar Weight)",asMETHOD(Entity,SetWeight1));
        Sys->RegisterClassMethod("Entity","void SetWeight3(int Idx,Scalar Weight)",asMETHOD(Entity,SetWeight2));
        Sys->RegisterClassMethod("Entity","void SetWeight4(int Idx,Scalar Weight)",asMETHOD(Entity,SetWeight3));
        Sys->RegisterClassMethod("Entity","Scalar GetWeight1(int Idx)",asMETHOD(Entity,GetWeight0));
        Sys->RegisterClassMethod("Entity","Scalar GetWeight2(int Idx)",asMETHOD(Entity,GetWeight1));
        Sys->RegisterClassMethod("Entity","Scalar GetWeight3(int Idx)",asMETHOD(Entity,GetWeight2));
        Sys->RegisterClassMethod("Entity","Scalar GetWeight4(int Idx)",asMETHOD(Entity,GetWeight3));
        Sys->RegisterClassMethod("Entity","void AddBoneIndex1(int Weight)",asMETHOD(Entity,AddVertexBoneIndex1));
        Sys->RegisterClassMethod("Entity","void AddBoneIndex2(int Weight0,int Weight1)",asMETHOD(Entity,AddVertexBoneIndex2));
        Sys->RegisterClassMethod("Entity","void AddBoneIndex3(int Weight0,int Weight1,int Weight2)",asMETHOD(Entity,AddVertexBoneIndex3));
        Sys->RegisterClassMethod("Entity","void AddBoneIndex4(int Weight0,int Weight1,int Weight2,int Weight3)",asMETHOD(Entity,AddVertexBoneIndex4));
        Sys->RegisterClassMethod("Entity","void SetBoneIndex1(int Idx,int bIdx)",asMETHOD(Entity,SetBoneIndex0));
        Sys->RegisterClassMethod("Entity","void SetBoneIndex2(int Idx,int bIdx)",asMETHOD(Entity,SetBoneIndex1));
        Sys->RegisterClassMethod("Entity","void SetBoneIndex3(int Idx,int bIdx)",asMETHOD(Entity,SetBoneIndex2));
        Sys->RegisterClassMethod("Entity","void SetBoneIndex4(int Idx,int bIdx)",asMETHOD(Entity,SetBoneIndex3));
        Sys->RegisterClassMethod("Entity","int GetBoneIndex1(int Idx)",asMETHOD(Entity,GetBoneIndex0));
        Sys->RegisterClassMethod("Entity","int GetBoneIndex2(int Idx)",asMETHOD(Entity,GetBoneIndex1));
        Sys->RegisterClassMethod("Entity","int GetBoneIndex3(int Idx)",asMETHOD(Entity,GetBoneIndex2));
        Sys->RegisterClassMethod("Entity","int GetBoneIndex4(int Idx)",asMETHOD(Entity,GetBoneIndex3));
        Sys->RegisterClassMethod("Entity","int GetVertexCount() const",asMETHOD(Entity,GetVertexCount));
        Sys->RegisterClassMethod("Entity","void SetTexParam(int TexNum,Glenum Param,Glenum Val)",asMETHOD(Entity,SetTexParam));
        Sys->RegisterClassMethod("Entity","void SetShader(const string& in) const",asMETHOD(Entity,SetShader));
        Sys->RegisterClassMethod("Entity","void SetPrimitiveType(int Type) const",asMETHOD(Entity,SetPrimType));
        Sys->RegisterClassMethod("Entity","int GetPrimitiveType() const",asMETHOD(Entity,GetPrimType));
        Sys->RegisterClassMethod("Entity","void AcquireDimensions()",asMETHOD(Entity,AcquireDimensions));
        Sys->RegisterClassMethod("Entity","void SetLightType(LIGHT_TYPE t)",asMETHOD(Entity,SetLightType));
        Sys->RegisterClassMethod("Entity","int GetLightType()",asMETHOD(Entity,GetLightType));
        Sys->RegisterClassMethod("Entity","void SetAmbient(const Vector4& in)",asMETHOD(Entity,SetAmbient));
        Sys->RegisterClassMethod("Entity","Vector4 GetAmbient()",asMETHOD(Entity,GetAmbient));
        Sys->RegisterClassMethod("Entity","void SetDiffuse(const Vector4& in)",asMETHOD(Entity,SetDiffuse));
        Sys->RegisterClassMethod("Entity","Vector4 GetDiffuse()",asMETHOD(Entity,GetDiffuse));
        Sys->RegisterClassMethod("Entity","void SetSpecular(const Vector4& in)",asMETHOD(Entity,SetSpecular));
        Sys->RegisterClassMethod("Entity","Vector4 GetSpecular()",asMETHOD(Entity,GetSpecular));
        Sys->RegisterClassMethod("Entity","void SetConstantAttenuation(Scalar Att)",asMETHOD(Entity,SetConstantAttenuation));
        Sys->RegisterClassMethod("Entity","Scalar GetConstantAttenuation() const",asMETHOD(Entity,GetConstantAttenuation));
        Sys->RegisterClassMethod("Entity","void SetLinearAttenuation(Scalar Att)",asMETHOD(Entity,SetLinearAttenuation));
        Sys->RegisterClassMethod("Entity","Scalar GetLinearAttenuation() const",asMETHOD(Entity,GetLinearAttenuation));
        Sys->RegisterClassMethod("Entity","void SetQuadraticAttenuation(Scalar Att)",asMETHOD(Entity,SetQuadraticAttenuation));
        Sys->RegisterClassMethod("Entity","Scalar GetQuadratcAttenuation() const",asMETHOD(Entity,GetQuadraticAttenuation));
        Sys->RegisterClassMethod("Entity","void SetAttenuation(Scalar C,Scalar L,Scalar Q)",asMETHOD(Entity,SetAttenuation));
        Sys->RegisterClassMethod("Entity","void SetExponent(Scalar Exp)",asMETHOD(Entity,SetExponent));
        Sys->RegisterClassMethod("Entity","Scalar GetExponent() const",asMETHOD(Entity,GetExponent));
        Sys->RegisterClassMethod("Entity","void SetIntensity(Scalar Intensity)",asMETHOD(Entity,SetIntensity));
        Sys->RegisterClassMethod("Entity","Scalar GetIntensity() const",asMETHOD(Entity,GetIntensity));
        Sys->RegisterClassMethod("Entity","void SetCutoffAngle(Scalar Cutoff)",asMETHOD(Entity,SetCutoffAngle));
        Sys->RegisterClassMethod("Entity","Scalar GetCutoffAngle() const",asMETHOD(Entity,GetCutoffAngle));
        Sys->RegisterClassMethod("Entity","void Identity()",asMETHOD(Entity,Identity));
        Sys->RegisterClassMethod("Entity","void Translate(const Vector3& in)",asMETHODPR(Entity,Translate,(const Vector3&),void));
        Sys->RegisterClassMethod("Entity","void Translate(Scalar x,Scalar y,Scalar z)",asMETHODPR(Entity,Translate,(Scalar,Scalar,Scalar),void));
        Sys->RegisterClassMethod("Entity","void Rotate(const Quaternion& in)",asMETHODPR(Entity,Rotate,(const Quaternion&),void));
        Sys->RegisterClassMethod("Entity","void Rotate(const Vector3& in,Scalar a)",asMETHODPR(Entity,Rotate,(const Vector3&,Scalar),void));
        Sys->RegisterClassMethod("Entity","void Rotate(Scalar x,Scalar y,Scalar z,Scalar a)",asMETHODPR(Entity,Rotate,(Scalar,Scalar,Scalar,Scalar),void));
        Sys->RegisterClassMethod("Entity","void Scale(Scalar s)",asMETHODPR(Entity,Scale,(Scalar),void));
        Sys->RegisterClassMethod("Entity","void Scale(Scalar x,Scalar y,Scalar z)",asMETHODPR(Entity,Scale,(Scalar,Scalar,Scalar),void));
        Sys->RegisterClassMethod("Entity","void Scale(const Vector3& in)",asMETHODPR(Entity,Scale,(const Vector3&),void));
        Sys->RegisterClassMethod("Entity","Vector3 GetPosition() const",asMETHOD(Entity,GetPosition));
        Sys->RegisterClassMethod("Entity","Vector3 GetScale() const",asMETHOD(Entity,GetScale));
        Sys->RegisterClassMethod("Entity","Quaternion GetOrientation() const",asMETHOD(Entity,GetOrientation));
        Sys->RegisterClassMethod("Entity","const Matrix4& GetTransform()",asMETHOD(Entity,GetTransform));
        Sys->RegisterClassMethod("Entity","void SetGravity(const Vector3& in)",asMETHOD(Entity,SetGravity));
        Sys->RegisterClassMethod("Entity","void ApplyImpulse(const Vector3& in,const Vector3& in)",asMETHOD(Entity,ApplyImpulse));
        Sys->RegisterClassMethod("Entity","void ApplyTorque(const Vector3& in)",asMETHOD(Entity,ApplyTorque));
        Sys->RegisterClassMethod("Entity","void ApplyTorqueImpulse(const Vector3& in)",asMETHOD(Entity,ApplyTorqueImpulse));
        Sys->RegisterClassMethod("Entity","void SetAngularDamping(Scalar Damping)",asMETHOD(Entity,SetAngularDamping));
        Sys->RegisterClassMethod("Entity","Scalar GetAngularDamping() const",asMETHOD(Entity,GetAngularDamping));
        Sys->RegisterClassMethod("Entity","void SetLinearDamping(Scalar Damping)",asMETHOD(Entity,SetLinearDamping));
        Sys->RegisterClassMethod("Entity","Scalar GetLinearDamping() const",asMETHOD(Entity,GetLinearDamping));
        Sys->RegisterClassMethod("Entity","Vector3 GetLinearVelocity() const",asMETHOD(Entity,GetLinearVelocity));
        Sys->RegisterClassMethod("Entity","Vector3 GetAngularVelocity() const",asMETHOD(Entity,GetAngularVelocity));
        Sys->RegisterClassMethod("Entity","void SetAngularFactor(const Vector3& in)",asMETHOD(Entity,SetAngularFactor));
        Sys->RegisterClassMethod("Entity","Vector3 GetAngularFactor() const",asMETHOD(Entity,GetAngularFactor));
        Sys->RegisterClassMethod("Entity","void SetSleepWhenInactive(bool DoSleep)",asMETHOD(Entity,SetSleepWhenInactive));
        Sys->RegisterClassMethod("Entity","void SetNear(Scalar Near)",asMETHOD(Entity,SetNear));
        Sys->RegisterClassMethod("Entity","void SetFar(Scalar Far)",asMETHOD(Entity,SetFar));
        Sys->RegisterClassMethod("Entity","Scalar GetNear()",asMETHOD(Entity,GetNear));
        Sys->RegisterClassMethod("Entity","Scalar GetFar()",asMETHOD(Entity,GetFar));
        Sys->RegisterClassMethod("Entity","void SetPlanes(Scalar Near,Scalar Far)",asMETHOD(Entity,SetPlanes));
        Sys->RegisterClassMethod("Entity","void SetExposure(Scalar Exp)",asMETHOD(Entity,SetExposure));
        Sys->RegisterClassMethod("Entity","Scalar GetExposure()",asMETHOD(Entity,GetExposure));
        Sys->RegisterClassMethod("Entity","void SetFieldOfView(Scalar FOV)",asMETHOD(Entity,SetFieldOfView));
        Sys->RegisterClassMethod("Entity","Scalar GetFieldOfView()",asMETHOD(Entity,GetFieldOfView));
        Sys->RegisterClassMethod("Entity","void SubscribeToInput()",asMETHOD(Entity,SubscribeToInput));
        Sys->RegisterClassMethod("Entity","void Send(const Event& in) const",asMETHOD(Entity,Send));
        
        /* File class */
        Sys->RegisterClass("File",sizeof(Entity),asOBJ_REF);
        Sys->RegisterClassBehavior("File",asBEHAVE_ADDREF,"void f()",asMETHOD(File,AddRef),asCALL_THISCALL);
        Sys->RegisterClassBehavior("File",asBEHAVE_RELEASE,"void f()",asMETHOD(File,Destroy),asCALL_THISCALL);
        Sys->RegisterClassMethod("File","int64 GetSize() const",asMETHOD(File,GetSize));
        Sys->RegisterClassMethod("File","char GetC() const",asMETHOD(File,GetC));
        Sys->RegisterClassMethod("File","string ReadLn() const",asMETHOD(File,ReadLn));
        Sys->RegisterClassMethod("File","int64 GetPosition() const",asMETHOD(File,GetPosition));
        Sys->RegisterClassMethod("File","void SetPosition(int64 Pos)",asMETHOD(File,SetPosition));
        Sys->RegisterClassMethod("File","bool AtEnd() const",asMETHOD(File,AtEnd));
        Sys->RegisterClassMethod("File","Scalar ParseScalar()",asMETHOD(File,ParseScalar));
        Sys->RegisterClassMethod("File","int ParseInt()",asMETHOD(File,ParseInt));
        Sys->RegisterClassMethod("File","bool ParseBool()",asMETHOD(File,ParseBool));
        Sys->RegisterClassMethod("File","Vector2 ParseVector2()",asMETHOD(File,ParseVector2));
        Sys->RegisterClassMethod("File","Vector3 ParseVector3()",asMETHOD(File,ParseVector3));
        Sys->RegisterClassMethod("File","Vector4 ParseVector4()",asMETHOD(File,ParseVector4));
        
        /* Entity controller interface */
        Sys->RegisterInterface("EntityController");
        Sys->RegisterInterfaceMethod("EntityController","void _Init(Entity@ E)");
        
        /* Engine enums */
        Sys->RegisterEnum("MESSAGE_TYPE"); //Only add 0-param message types
        Sys->RegisterEnumValue("MESSAGE_TYPE","SHUTDOWN",MT_SHUTDOWN);
        
        /* Engine class */
        Sys->RegisterClass("Engine",sizeof(Engine),asOBJ_POD | asOBJ_VALUE);
        Sys->RegisterClassMethod("Engine","Entity@ GetEntity(int UID)"                          ,asMETHOD(Engine,GetEntity));
        Sys->RegisterClassMethod("Engine","void SendMessage(MESSAGE_TYPE Type)"                 ,asMETHODPR(Engine,SendMessage,(MESSAGE_TYPE),void));
        Sys->RegisterClassMethod("Engine","void SendEvent(const Event& in)"                     ,asMETHODPR(Engine,SendEvent,(const Event&),void));
        Sys->RegisterClassMethod("Engine","void SendEvent(Entity@ E,const Event& in)"           ,asMETHODPR(Engine,SendEvent,(Entity*,const Event&),void));
        Sys->RegisterClassMethod("Engine","Entity@ CreateScriptedEntity(const string& in,Scalar Rate = 0)",asMETHOD(Engine,CreateScriptedEntity));
        Sys->RegisterClassMethod("Engine","Entity@ Spawn(const string& in)"                     ,asMETHOD(Engine,SpawnEntity));
        Sys->RegisterClassMethod("Engine","Entity@ Spawn(const string& in,const Vector3& in)"   ,asMETHOD(Engine,SpawnEntityPos));
        Sys->RegisterClassMethod("Engine","Entity@ CreateEntity()"                              ,asMETHOD(Engine,CreateEntity));
        Sys->RegisterClassMethod("Engine","int Execute(const string& in)"                       ,asMETHOD(Engine,Execute));
        Sys->RegisterClassMethod("Engine","void MakeRenderable(Entity@ E)"                      ,asMETHOD(Engine,MakeRenderable));
        Sys->RegisterClassMethod("Engine","void MakeTransformable(Entity@ E)"                   ,asMETHOD(Engine,MakeTransformable));
        Sys->RegisterClassMethod("Engine","void MakeCamera(Entity@ E)"                          ,asMETHOD(Engine,MakeObserver));
        Sys->RegisterClassMethod("Engine","void MakePhysical(Entity@ E)"                        ,asMETHOD(Engine,MakePhysical));
        Sys->RegisterClassMethod("Engine","void MakeLight(Entity@ E)"                           ,asMETHOD(Engine,MakeLight));
        Sys->RegisterClassMethod("Engine","Entity@ GetCurrentCamera()"                          ,asMETHOD(Engine,GetCurrentCamera));
        Sys->RegisterClassMethod("Engine","int GetEntityCount() const"                          ,asMETHOD(Engine,GetEntityCount));
        Sys->RegisterClassMethod("Engine","int GetVisibleEntityCount() const"                   ,asMETHOD(Engine,GetVisibleEntityCount));
        Sys->RegisterClassMethod("Engine","Scalar GetRunTime() const"                           ,asMETHOD(Engine,GetRunTime));
        Sys->RegisterClassMethod("Engine","void SetGameTimeRate(Scalar Rate)"                   ,asMETHOD(Engine,SetGameTimeRate));
        Sys->RegisterClassMethod("Engine","Scalar GetGameTimeRate() const"                      ,asMETHOD(Engine,GetGameTimeRate));
        Sys->RegisterClassMethod("Engine","Scalar SetTimeOfDay(Scalar T)"                       ,asMETHOD(Engine,SetTimeOfDay));
        Sys->RegisterClassMethod("Engine","Scalar GetTimeOfDay() const"                         ,asMETHOD(Engine,GetTimeOfDay));
        Sys->RegisterClassMethod("Engine","Scalar GetFramerate() const"                         ,asMETHOD(Engine,GetFramerate));
        Sys->RegisterClassMethod("Engine","Vector3 GetResolution() const"                       ,asMETHOD(Engine,GetResolution));
        Sys->RegisterClassMethod("Engine","void ToggleDebugCullingDisplay()"                    ,asMETHOD(Engine,ToggleDebugCullingDisplay));
        Sys->RegisterClassMethod("Engine","void ToggleDebugPhysicsDisplay()"                    ,asMETHOD(Engine,ToggleDebugPhysicsDisplay));
        Sys->RegisterClassMethod("Engine","bool VarExists(const string& in)"                    ,asMETHOD(Engine,VarExists         ));
        Sys->RegisterClassMethod("Engine","void AddVar(const string& in,Scalar Val)"            ,asMETHOD(Engine,AddGameVariablef  ));
        Sys->RegisterClassMethod("Engine","void AddVar(const string& in,int Val)"               ,asMETHOD(Engine,AddGameVariablei  ));
        Sys->RegisterClassMethod("Engine","void AddVar(const string& in,bool Val)"              ,asMETHOD(Engine,AddGameVariableb  ));
        Sys->RegisterClassMethod("Engine","void AddVar(const string& in,const Vector2& in)"     ,asMETHOD(Engine,AddGameVariablefv2));
        Sys->RegisterClassMethod("Engine","void AddVar(const string& in,const Vector3& in)"     ,asMETHOD(Engine,AddGameVariablefv3));
        Sys->RegisterClassMethod("Engine","void AddVar(const string& in,const Vector4& in)"     ,asMETHOD(Engine,AddGameVariablefv4));
        Sys->RegisterClassMethod("Engine","void AddVar(const string& in,const string& in)"      ,asMETHOD(Engine,AddGameVariableStr));
        Sys->RegisterClassMethod("Engine","void SetVar(const string& in,Scalar Val)"            ,asMETHOD(Engine,SetGameVariablef  ));
        Sys->RegisterClassMethod("Engine","void SetVar(const string& in,int Val)"               ,asMETHOD(Engine,SetGameVariablei  ));
        Sys->RegisterClassMethod("Engine","void SetVar(const string& in,bool Val)"              ,asMETHOD(Engine,SetGameVariableb  ));
        Sys->RegisterClassMethod("Engine","void SetVar(const string& in,const Vector2& in)"     ,asMETHOD(Engine,SetGameVariablefv2));
        Sys->RegisterClassMethod("Engine","void SetVar(const string& in,const Vector3& in)"     ,asMETHOD(Engine,SetGameVariablefv3));
        Sys->RegisterClassMethod("Engine","void SetVar(const string& in,const Vector4& in)"     ,asMETHOD(Engine,SetGameVariablefv4));
        Sys->RegisterClassMethod("Engine","void SetVar(const string& in,const string& in)"      ,asMETHOD(Engine,SetGameVariableStr));
        Sys->RegisterClassMethod("Engine","Scalar GetVarf(const string& in)"                    ,asMETHOD(Engine,GetGameVariablef  ));
        Sys->RegisterClassMethod("Engine","int GetVari(const string& in)"                       ,asMETHOD(Engine,GetGameVariablei  ));
        Sys->RegisterClassMethod("Engine","bool GetVarb(const string& in)"                      ,asMETHOD(Engine,GetGameVariableb  ));
        Sys->RegisterClassMethod("Engine","Vector2 GetVarv2(const string& in)"                  ,asMETHOD(Engine,GetGameVariablefv2));
        Sys->RegisterClassMethod("Engine","Vector3 GetVarv3(const string& in)"                  ,asMETHOD(Engine,GetGameVariablefv3));
        Sys->RegisterClassMethod("Engine","Vector4 GetVarv4(const string& in)"                  ,asMETHOD(Engine,GetGameVariablefv4));
        Sys->RegisterClassMethod("Engine","string GetVarStr(const string& in)"                  ,asMETHOD(Engine,GetGameVariableStr));
        Sys->RegisterClassMethod("Engine","File@ OpenFile(const string& in,const string& in)"   ,asMETHOD(Engine,OpenFile));
        Sys->RegisterClassMethod("Engine","void CloseFile(File@ fp)"                            ,asMETHOD(Engine,CloseFile));
        
        Sys->RegisterVariable("Engine Game",Engine::GetEngine());
        
    }
};
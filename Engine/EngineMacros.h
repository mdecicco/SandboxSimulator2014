/*
 *  EngineMacros.h
 *
 *  Created by Michael DeCicco on 6/23/14.
 *
 */

#pragma once

#define MakeCString(s) new char[s]
#define FreeCString(s) delete [] s

#define GetRenderComponent(E) ((RenderComponent*)E->GetComponentByType(CT_RENDER))
#define GetScriptComponent(E) ((ScriptComponent*)E->GetComponentByType(CT_SCRIPT))
#define GetTransformComponent(E) ((TransformComponent*)E->GetComponentByType(CT_TRANSFORM))
#define GetLightComponent(E) ((LightComponent*)E->GetComponentByType(CT_LIGHT))
#define GetCameraComponent(E) ((CameraComponent*)E->GetComponentByType(CT_CAMERA))

#define Profile(Stuff) { Timer _tmr; _tmr.Start(); Stuff; Log("%s Executed in %0.2f ms.\n",#Stuff,((Scalar)_tmr) * 1000.0f); }
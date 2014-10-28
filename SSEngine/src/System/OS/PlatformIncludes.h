#ifndef PLATFORM_INCLUDES_H
#define PLATFORM_INCLUDES_H

#define GLEW_STATIC
#define BC_USE_GLFW_BACKEND

#include <System/BcTypes.h>


#include <GL/glew.h>

#ifdef BC_USE_GLFW_BACKEND
 #include <System/OS/GlfwBackend.h>
#endif

#ifdef BC_USE_WINDOWS_BACKEND
#include <System/OS/WindowsBackend.h>
#endif

#ifdef BC_USE_X11_BACKEND
 #include <System/OS/X11Backend.h>
#endif

#include <Utils/EngineMacros.h>
#include <memory>

#include <memory>

#include <malloc.h>

#endif

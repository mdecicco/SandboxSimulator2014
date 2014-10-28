#ifndef PLATFORM_INCLUDES_H
#define PLATFORM_INCLUDES_H

#define GLEW_STATIC
#define SS_USE_GLFW_BACKEND

#include <System/SSTypes.h>

#include <GL/glew.h>

#ifdef SS_USE_GLFW_BACKEND
 #include <System/OS/GlfwBackend.h>
#endif

#ifdef SS_USE_X11_BACKEND
 #include <System/OS/X11Backend.h>
#endif

#include <memory>

#include <memory>

#include <malloc.h>

#endif

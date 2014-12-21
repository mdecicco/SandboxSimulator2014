#ifndef PLATFORM_INCLUDES_H
#define PLATFORM_INCLUDES_H

#define GLEW_STATIC

#include <System/SSTypes.h>

#include <GL/glew.h>

#include <memory>

#ifdef __APPLE__
    #include <sys/malloc.h>
#else
    #include <malloc.h>
#endif
#endif

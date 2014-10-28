#ifndef DEBUG_SINGLETON
#define DEBUG_SINGLETON

#ifdef __APPLE__
#define AllocArray(Type,Size) (Type *)__alloca(Size * sizeof(Type))
#else
#define AllocArray(Type,Size) (Type *)malloc(Size * sizeof(Type))
#endif

#define SS_LOG DebugSingleton::GetDebug()->Log
#define SS_ERROR(...) {DebugSingleton::GetDebug()->Log(__VA_ARGS__); abort(); }
#define AllocStr(Sz) AllocArray(char,Sz)

#include <stdio.h>
#include <iostream>
#include <stdarg.h>

namespace SSEngine {
    class Logger;

class DebugSingleton
{
private:
    DebugSingleton() {}

protected:
    FILE* m_Log;

public:
    ~DebugSingleton();
    static DebugSingleton* GetDebug() {
        static DebugSingleton* debug = new DebugSingleton();
        return debug;
    }

    i32 Log(const char* Text, ...) {
        va_list List;
        va_start(List, Text);
        int Size = strlen(Text) + 512;
        char *Formatted = AllocStr(Size);

        vsnprintf(Formatted, Size, Text, List);
        va_end(List);

		i32 Ret = 0;// fprintf(m_Log, "%s", Formatted);
        printf("%s", Formatted);

        free(Formatted);
        return Ret;
    }
};
}

#endif

#include <iostream>
#include <stdio.h>

#include <Engine.h>

using namespace SandboxSimulator;

int main(i32 ArgC,Literal ArgV[])
{
    SSEngine* Eng = new SSEngine();
    
    Eng->Initialize(ArgC,ArgV);
    Eng->Run();
    Eng->Shutdown();
    return 0;
}

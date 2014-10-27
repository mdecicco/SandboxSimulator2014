/*
 *  main.cpp
 *
 *  Created by Michael DeCicco on 6/15/14.
 *
 */
#include "Engine.h"
using namespace Reality;

int main(i32 ArgC,Literal ArgV[])
{
    //Construct the engine
    Engine::CreateEngine();
    Engine* Eng = Engine::GetEngine();
    
    //Initialize, run, shutdown
    Eng->Initialize(ArgC,ArgV);
    Eng->Run();
    Eng->Shutdown();
    
    return 0;
}
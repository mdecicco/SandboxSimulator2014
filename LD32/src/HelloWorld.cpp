#include <HelloWorld.h>

namespace LudumDare {
    HelloWorld::HelloWorld() {

    }

    HelloWorld::~HelloWorld() {

    }

    void HelloWorld::Init(StateManagerSystem* Manager) {
        m_Manager = Manager;
    }

    void HelloWorld::Update(Scalar DeltaTime) {
        m_Engine->Log("Hello World");
    }
}
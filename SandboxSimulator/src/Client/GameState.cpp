#include <Client/GameState.h>
#include <Engine.h>

#include <Core/TransformComponent.h>

namespace SandboxSimulator
{
    GameState::GameState(ConnectionSystem* Conn, std::string Address, u16 Port) : m_Connection(Conn), m_LastPacketTime(0), m_Port(Port), m_Address(Address), MouseHeld(false)
    {}

    GameState::~GameState()
    {}

    void GameState::Init(StateManagerSystem *StateManager)
    {
        m_StateManager = StateManager;
        m_Connection->Connect(m_Address, m_Port);
        Entity* E = m_Engine->GetSceneGraph()->CreateEntity(999);
        m_Engine->GetSceneGraph()->AddComponent(E, new RenderComponent());
        m_Engine->GetSceneGraph()->AddComponent(E, new TransformComponent());
        RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);
        r->SetShape(RC_SKY_SPHERE);

        Entity* G = m_Engine->GetSceneGraph()->CreateEntity(998);
        m_Engine->GetSceneGraph()->AddComponent(G, new RenderComponent());
        m_Engine->GetSceneGraph()->AddComponent(G, new TransformComponent());
        RenderComponent* gr = (RenderComponent*)G->GetComponentByType(CT_RENDER);
        gr->SetShape(RC_GROUND_PLANE);
        TransformComponent* gt = (TransformComponent*)G->GetComponentByType(CT_TRANSFORM);
        gt->SetPosition(Vec3(0,-2,0));

        Entity* Cam = m_Engine->GetSceneGraph()->CreateEntity(997);
        CamTrans = new TransformComponent();
        m_Engine->GetSceneGraph()->AddComponent(Cam, CamTrans);
        m_Engine->GetSceneGraph()->AddComponent(Cam, new CameraComponent());
        m_Engine->GetRenderSystem()->SetCamera(Cam);
        CamTrans->Translate(0,1.2,1.5);
        CamTrans->Rotate(1,0,0,-20);

        m_Engine->GetInputSystem()->SetMousePosition(Vec2(400,300));
    }

    void GameState::Update(Scalar dt)
    {
        //m_Engine->Log("Hello world?\n");
        Vec2 MousePos = Vec2(0,0);
        if(m_Engine->GetInputSystem()->ButtonDown(GLFW_MOUSE_BUTTON_1)) {
            if(!MouseHeld)
                m_Engine->GetInputSystem()->SetMousePosition(Vec2(400,300));
            MouseHeld = true;
        } else {
            MouseHeld = false;
        }
        if(MouseHeld) {
            MousePos = m_Engine->GetInputSystem()->GetMousePosition();
            MousePos.x -= 400;
            MousePos.y -= 300;
            m_Engine->GetInputSystem()->SetMousePosition(Vec2(400,300));
        }
        
        CamTrans->Rotate(Vec3(1,0,0), -MousePos.y*0.5);

        if(m_Connection->IsConnected() && m_Engine->GetSceneGraph()->HasEntity(m_Connection->GetEntityID())) {
            CamTrans->SetRelativeTo(m_Engine->GetSceneGraph()->GetEntityById(m_Connection->GetEntityID()));
            Scalar pDelta = m_Engine->GetElapsedTime() - m_LastPacketTime;
            TransformComponent* trans = (TransformComponent*)m_Engine->GetSceneGraph()->GetEntityById(m_Connection->GetEntityID())->GetComponentByType(CT_TRANSFORM);
            Vec3 Pos = Vec3();//trans->GetPosition();

            if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_W)) {
                Pos += Vec3(0,0,-2*dt);
                m_Connection->NeedsUpdate(true);
            } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_S)) {
                Pos += Vec3(0,0,2*dt);
                m_Connection->NeedsUpdate(true);
            } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_A)) {
                Pos += Vec3(-2*dt,0,0);
                m_Connection->NeedsUpdate(true);
            } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_D)) {
                Pos += Vec3(2*dt,0,0);
                m_Connection->NeedsUpdate(true);
            }

            trans->Translate(Pos);
            trans->Rotate(Vec3(0,1,0), -MousePos.x*0.5f);
            if(pDelta > 0.05) {
                m_Connection->SendPositionUpdate(trans->GetPosition());
                m_LastPacketTime = m_Engine->GetElapsedTime();
            }
        }
    }
}
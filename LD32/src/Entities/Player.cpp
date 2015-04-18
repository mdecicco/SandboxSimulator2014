#include <Entities/Player.h>

namespace LudumDare {
    Player::Player() 
    {
        //This class is really more just to move code out of the state
    }

    Player::~Player() 
    {
        //Everything should be cleaned up by the scene graph already
    }

    void Player::Init(SSEngine* Eng) 
    {
        m_Engine = Eng;

        m_Body = Eng->GetSceneGraph()->CreateEntity();
        m_Camera = Eng->GetSceneGraph()->CreateEntity();

        //Setup the body
        m_Engine->GetSceneGraph()->AddComponent(m_Body, new RenderComponent());
        m_Engine->GetSceneGraph()->AddComponent(m_Body, new TransformComponent());
        GetRender()->SetShape(RC_POINT_SPRITE);
        m_Engine->GetPhysicsSystem()->MakePhysical(m_Body, 1.0, true, 0.0f, 2.0f);
        PhysicsComponent* Col = GetPhysics();
        Col->SetSleepWhenActive(false);
        Col->SetAngularFactor(Vec3(0,0,0));

        //Setup the camera
        m_Engine->GetSceneGraph()->AddComponent(m_Camera, new CameraComponent());
        m_Engine->GetSceneGraph()->AddComponent(m_Camera, new TransformComponent());
        TransformComponent* Trans = GetCameraTransform();
        Trans->SetPosition(Vec3(0,1,3));
        Trans->SetFirstPerson(true);
        Trans->Rotate(1,0,0,-20);
        Trans->SetRelativeTo(m_Body);
    }

    void Player::Update(Scalar dt) 
    {
        Vec3 Delta = Vec3();
        Scalar speed = 50 * dt;

        if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_W)) {
            Delta += Vec3(0,0,-speed);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_S)) {
            Delta += Vec3(0,0,speed);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_A)) {
            Delta += Vec3(-speed,0,0);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_D)) {
            Delta += Vec3(speed,0,0);
        } if(m_Engine->GetInputSystem()->KeyDown(GLFW_KEY_SPACE)) {
            Delta += Vec3(0,speed,0);
        }

        Vec2 MousePos = m_Engine->GetInputSystem()->GetMouseDelta();
        GetTransform()->Rotate(Vec3(0,1,0),-MousePos.x*0.5f);
        GetTransform()->Rotate(Vec3(1,0,0),-MousePos.y*0.5f);
        GetTransform()->AddImpulse(Delta, true);
    }
}
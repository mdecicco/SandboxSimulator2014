#ifndef SandboxSimulator_PhysicsSystem_h
#define SandboxSimulator_PhysicsSystem_h

#include <Core/EngineSystem.h>
#include <Core/Timer.h>
#include <System/SSThread.h>
#include <System/SSTypes.h>
#include <Core/SceneGraph.h>
#include <Utils/Math.h>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

namespace SandboxSimulator {
class SSEngine;

class PhysicsComponent : public Component
{
    public:
        PhysicsComponent();
        ~PhysicsComponent();

        btRigidBody* m_Body;
        UID m_EntityID;
        bool m_Disabled;

        void SetGravity(Vec3 Grav)
        { 
            m_Body->setGravity(Grav); 
        }
        
        void ApplyImpulse(Vec3 Impulse, Vec3 Rel)
        { 
            m_Body->applyImpulse(Impulse, Rel);
        }

        void ApplyForce(Vec3 Force, Vec3 Rel)
        {
            m_Body->applyForce(Force, Rel);
        }

        void ApplyCentralForce(Vec3 Force)
        {
            m_Body->applyCentralForce(Force);
        }

        void SetSleepWhenActive(bool Sleep)
        {
            if(!Sleep) m_Body->setActivationState(DISABLE_DEACTIVATION);
            else m_Body->setActivationState(WANTS_DEACTIVATION);
        }

        void SetAngularFactor(Vec3 Factor)
        {
            m_Body->setAngularFactor(Factor);
        }
};

class PhysicsSystem : public EngineSystem
{
    public:
        PhysicsSystem();
        ~PhysicsSystem();
    
        virtual void HandleMessage(const EngineMessage* Msg);
    
        virtual void Initialize(SSEngine* eng);
        virtual void Update(Scalar dt);
        virtual void Shutdown();
    
        virtual void Serialize();
        virtual void Deserialize();
    
        btDiscreteDynamicsWorld* GetWorld() const { return m_World; }
    
        //Frame* GetFrame();
    
        void MakePhysical(Entity* E, Scalar Mass, bool Sphere, Scalar Restitution = 1.5f, Scalar Friction = 1.5f);
        void ProcessEvent();
    
        void SetTerrain(Vec3* Vertices,u32 HeightmapSize,Scalar MaxHeight,Scalar Scale);
    
    protected:
        btBroadphaseInterface* m_Broadphase;
        btDefaultCollisionConfiguration* m_DefaultCollisionConfig;
        btCollisionDispatcher* m_Dispatcher;
        btSequentialImpulseConstraintSolver* m_Solver;
        btDiscreteDynamicsWorld* m_World;
        btRigidBody* m_Terrain;
        Scalar m_dt;
        Scalar m_FrameTimestamp;
        Scalar m_LastFrameTimestamp;
        SSEngine* m_Engine;
        //FrameID m_FrameID;
        //sf::Mutex* m_FrameMutex;
        //vector<Frame*> m_Frames;
        //Mutex m_MessageMutex;
        //vector<EngineMessage*> m_PendingMessages;
};
}

#endif
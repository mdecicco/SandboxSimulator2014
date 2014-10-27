/*
 *  PhysicsSystem.h
 *
 *  Created by Michael DeCicco on 9/8/14.
 *
 */

#pragma once
#include "EngineSystem.h"
#include "Timer.h"
#include "btBulletDynamicsCommon.h"

namespace Reality
{
    class PhysicsSystem : public EngineSystem
    {
        public:
            PhysicsSystem();
            ~PhysicsSystem();
        
            virtual void HandleMessage(const EngineMessage* Msg);
        
            virtual void Initialize();
            virtual void Update(Scalar dt);
            virtual void Shutdown();
        
            virtual void Serialize();
            virtual void Deserialize();
        
            btDiscreteDynamicsWorld* GetWorld() const { return m_World; }
        
        protected:
            btBroadphaseInterface* m_Broadphase;
            btDefaultCollisionConfiguration* m_DefaultCollisionConfig;
            btCollisionDispatcher* m_Dispatcher;
            btSequentialImpulseConstraintSolver* m_Solver;
            btDiscreteDynamicsWorld* m_World;
            Timer m_Tmr;
    };
};
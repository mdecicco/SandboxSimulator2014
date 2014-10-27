/*
 *  PhysicsSystem.cpp
 *
 *  Created by Michael DeCicco on 9/8/14.
 *
 */

#include "PhysicsSystem.h"
#include "btBulletDynamicsCommon.h"
#include "btShapeHull.h"
#include "Message.h"
#include "RenderSystem.h"
#include "Engine.h"

namespace Reality
{
    PhysicsSystem::PhysicsSystem()
    {
    }
    PhysicsSystem::~PhysicsSystem()
    {
    }

    void PhysicsSystem::HandleMessage(const EngineMessage* Msg)
    {
        switch(Msg->m_MessageType)
        {
            case MT_MAKE_PHYSICAL:
            {
                MakePhysicalMessage* pMsg = (MakePhysicalMessage*)Msg;
                TransformComponent* t = (TransformComponent*)pMsg->m_Entity->GetComponentByType(CT_TRANSFORM);
                RenderComponent   * r = (RenderComponent   *)pMsg->m_Entity->GetComponentByType(CT_RENDER   );
                if(!t) return;
                if(r && r->GetVertexCount() >= 0)
                {
                    if(pMsg->Shape.length() > 0)
                    {
                        if(pMsg->Shape == "SPHERE")
                        {
                            if(t->IsStatic())
                            {
                                btSphereShape* Shape = new btSphereShape(t->GetBoundingVolume().Dimensions.Magnitude() * 0.5f);
                        
                                btDefaultMotionState* State = new btDefaultMotionState(btTransform(t->GetOrientation(),t->GetPosition()));
                                btRigidBody::btRigidBodyConstructionInfo BodyCI(0.0f,State,Shape,btVector3(0,0,0));
                                btRigidBody* Body = new btRigidBody(BodyCI);
                                m_World->addRigidBody(Body);
                                t->m_Body = Body;
                                Body->setRestitution(pMsg->Restitution);
                                Body->setFriction(pMsg->Friction);
                            }
                            else
                            {
                                btSphereShape* Shape = new btSphereShape(r->GetDimensions().y * 0.5f);
                        
                                btDefaultMotionState* State = new btDefaultMotionState(btTransform(t->GetOrientation(),t->GetPosition()));
                                btVector3 Inertia(0, 0, 0);
                                Shape->calculateLocalInertia(pMsg->Mass,Inertia);
                                btRigidBody::btRigidBodyConstructionInfo BodyCI(pMsg->Mass,State,Shape,Inertia);
                                btRigidBody* Body = new btRigidBody(BodyCI);
                                m_World->addRigidBody(Body);
                                t->m_Body = Body;
                                Body->setRestitution(pMsg->Restitution);
                                Body->setFriction(pMsg->Friction);
                            }
                            return;
                        }
                        else
                        {
                            Log("Warning: Unsupported shape. Using hull.\n");
                        }
                    }
                    btTriangleMesh* tMesh = new btTriangleMesh();
                    if(r->GetMesh()->GetIndexCount() == 0)
                    {
                        for(int i = 0;i < r->GetVertexCount();i += 3) tMesh->addTriangle(r->GetVertex(i+0),r->GetVertex(i+1),r->GetVertex(i+2));
                    }
                    else
                    {
                        for(i32 i = 0;i < r->GetMesh()->GetIndexCount();i += 3)
                        {
                            tMesh->addTriangle(r->GetVertex(r->GetMesh()->GetIndex(i+0)),
                                               r->GetVertex(r->GetMesh()->GetIndex(i+1)),
                                               r->GetVertex(r->GetMesh()->GetIndex(i+2)));
                        }
                    }
                    if(t->IsStatic())
                    {
                        btBvhTriangleMeshShape* Shape = new btBvhTriangleMeshShape(tMesh,true,true);
                        btDefaultMotionState* State = new btDefaultMotionState(btTransform(t->GetOrientation(),t->GetPosition()));
                        btRigidBody::btRigidBodyConstructionInfo BodyCI(0.0f,State,Shape,btVector3(0, 0, 0));
                        btRigidBody* Body = new btRigidBody(BodyCI);
                        Body->setRestitution(pMsg->Restitution);
                        Body->setFriction(pMsg->Friction);
                        m_World->addRigidBody(Body);
                        t->m_Body = Body;
                    }
                    else
                    {
                        btConvexShape *Shape = new btConvexTriangleMeshShape(tMesh);
                        btShapeHull *hull = new btShapeHull(Shape);
                        btScalar margin = Shape->getMargin();
                        hull->buildHull(margin);
                        Shape->setUserPointer(hull);
                        
                        btDefaultMotionState* State = new btDefaultMotionState(btTransform(t->GetOrientation(),t->GetPosition()));
                        btVector3 Inertia(0, 0, 0);
                        Shape->calculateLocalInertia(pMsg->Mass,Inertia);
                        btRigidBody::btRigidBodyConstructionInfo BodyCI(pMsg->Mass,State,Shape,Inertia);
                        btRigidBody* Body = new btRigidBody(BodyCI);
                        m_World->addRigidBody(Body);
                        t->m_Body = Body;
                        Body->setRestitution(pMsg->Restitution);
                        Body->setFriction(pMsg->Friction);
                    }
                }
                else
                {
                }
                
                break;
            }
            case MT_DESTROY_ENTITY:
            {
                break;
            }
            default:
            {
                break;
            }
        }
    }

    void PhysicsSystem::Initialize()
    {
        m_Broadphase = new btDbvtBroadphase();
        m_DefaultCollisionConfig = new btDefaultCollisionConfiguration();
        m_Dispatcher = new btCollisionDispatcher(m_DefaultCollisionConfig);
        m_Solver = new btSequentialImpulseConstraintSolver;
        m_World = new btDiscreteDynamicsWorld(m_Dispatcher,m_Broadphase,m_Solver,m_DefaultCollisionConfig);
        m_World->setGravity(btVector3(0,-9.80665f,0));
        m_Tmr.Start();
    }
    void PhysicsSystem::Update(Scalar dt)
    {
        Scalar Timestep = m_Tmr;
        m_Tmr.Stop();
        m_Tmr.Start();
        m_World->stepSimulation(Timestep,20,Engine::GetEngine()->GetTimestep() * 0.5f);
    }
    void PhysicsSystem::Shutdown()
    {
        delete m_World;
        delete m_Solver;
        delete m_Dispatcher;
        delete m_DefaultCollisionConfig;
        delete m_Broadphase;
    }

    void PhysicsSystem::Serialize()
    {
    }
    void PhysicsSystem::Deserialize()
    {
    }
};
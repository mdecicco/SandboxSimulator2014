#include <Core/PhysicsSystem.h>
#include <Engine.h>

#include <Core/TransformComponent.h>
#include <Rendering/RenderSystem.h>

#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <btBulletDynamicsCommon.h>

namespace SandboxSimulator
{
    PhysicsComponent::PhysicsComponent() : Component(CT_PHYSICS), m_Body(0), m_Disabled(false)
    {}

    PhysicsComponent::~PhysicsComponent()
    {
        delete m_Body->getMotionState();
        //if(m_Body->getCollisionShape()->getUserPointer() != nullptr) delete m_Body->getCollisionShape()->getUserPointer();
        if(m_Body->getCollisionShape()->getShapeType() == TRIANGLE_MESH_SHAPE_PROXYTYPE || m_Body->getCollisionShape()->getShapeType() == CONVEX_HULL_SHAPE_PROXYTYPE) {
            btBvhTriangleMeshShape* shp = (btBvhTriangleMeshShape*)m_Body->getCollisionShape();
            delete shp->getMeshInterface();
            if(!m_Body->isStaticObject())
                delete m_Body->getCollisionShape()->getUserPointer();
        }

        delete m_Body->getCollisionShape();
        delete m_Body;
    }

    PhysicsSystem::PhysicsSystem() : m_dt(0.0f), m_FrameTimestamp(0.0f), m_LastFrameTimestamp(0.0f)
    {
        AddComponentType(CT_PHYSICS);
    }

    PhysicsSystem::~PhysicsSystem()
    {}

    void PhysicsSystem::HandleMessage(const EngineMessage* Msg)
    {}

    void PhysicsSystem::Initialize(SSEngine* Eng)
    {
        m_Engine = Eng;
        m_Broadphase = new btDbvtBroadphase();
        m_DefaultCollisionConfig = new btDefaultCollisionConfiguration();
        m_Dispatcher = new btCollisionDispatcher(m_DefaultCollisionConfig);
        m_Solver = new btSequentialImpulseConstraintSolver;
        m_World = new btDiscreteDynamicsWorld(m_Dispatcher, m_Broadphase, m_Solver, m_DefaultCollisionConfig);
        m_World->setGravity(btVector3(0,-9.80665f,0));
    }

    void PhysicsSystem::Update(Scalar dt)
    {
        m_LastFrameTimestamp = m_FrameTimestamp;
        m_FrameTimestamp = m_Engine->GetTimeOfDay() * 86400.0f;
        m_dt = m_FrameTimestamp - m_LastFrameTimestamp;
        if(m_dt == 0.0f) m_dt = dt;
        if(m_dt < 0.0f) m_dt = 0.0000001f;
        //if(m_Engine->IsSimulationPaused()) return;
        m_World->stepSimulation(dt, 20);
        for(i32 i = 0;i < m_Components.size();i++)
        {
            if(((PhysicsComponent*)m_Components[i])->m_Disabled) continue;
            
            btRigidBody* Body = ((PhysicsComponent*)m_Components[i])->m_Body;
            btTransform Trans = Body->getWorldTransform();
            const btVector3   & t = Trans.getOrigin();
            const btQuaternion& r = Trans.getRotation();
            const btVector3   & s = Body->getCollisionShape()->getLocalScaling();
            TransformComponent* et = (TransformComponent*)m_Components[i]->GetParent()->GetComponentByType(CT_TRANSFORM);
            et->SetPosition(Vec3(t), false);
            et->SetOrientation(Quat(r), false);
            et->SetScale(Vec3(s));
        }
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
    {}

    void PhysicsSystem::Deserialize()
    {}

    void PhysicsSystem::MakePhysical(Entity* E, Scalar Mass, bool Sphere, Scalar Restitution, Scalar Friction)
    {
        TransformComponent* t = (TransformComponent*)E->GetComponentByType(CT_TRANSFORM);
        RenderComponent* r = (RenderComponent*)E->GetComponentByType(CT_RENDER);
        if(!E->HasComponentType(CT_TRANSFORM) || !E->HasComponentType(CT_RENDER)) return;

        btRigidBody* Body = 0;
        if(r->GetVertexCount() >= 0)
        {
            if(Sphere)
            {
                if(t->IsStatic())
                {
                    btSphereShape* Shape = new btSphereShape(0.5f);
                    Shape->setLocalScaling(t->GetScale());
            
                    btDefaultMotionState* State = new btDefaultMotionState(btTransform(t->GetOrientation(),t->GetPosition()));
                    btRigidBody::btRigidBodyConstructionInfo BodyCI(0.0f,State,Shape,btVector3(0,0,0));
                    Body = new btRigidBody(BodyCI);
                    m_World->addRigidBody(Body);
                    Body->setRestitution(Restitution);
                    Body->setFriction(Friction);
                }
                else
                {
                    btSphereShape* Shape = new btSphereShape(0.5f);
                    Shape->setLocalScaling(t->GetScale());
            
                    btDefaultMotionState* State = new btDefaultMotionState(btTransform(t->GetOrientation(),t->GetPosition()));
                    btVector3 Inertia(0, 0, 0);
                    Shape->calculateLocalInertia(Mass,Inertia);
                    btRigidBody::btRigidBodyConstructionInfo BodyCI(Mass,State,Shape,Inertia);
                    Body = new btRigidBody(BodyCI);
                    m_World->addRigidBody(Body);
                    Body->setRestitution(Restitution);
                    Body->setFriction(Friction);
                }
            } else {
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
                    Body = new btRigidBody(BodyCI);
                    Body->setRestitution(Restitution);
                    Body->setFriction(Friction);
                    m_World->addRigidBody(Body);
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
                    Shape->calculateLocalInertia(Mass,Inertia);
                    btRigidBody::btRigidBodyConstructionInfo BodyCI(Mass,State,Shape,Inertia);
                    Body = new btRigidBody(BodyCI);
                    m_World->addRigidBody(Body);
                    Body->setRestitution(Restitution);
                    Body->setFriction(Friction);
                }
            }
        } else {
            m_Engine->Log("Entity <%d> has no vertices, cannot be added to physics.", E->GetID());
        }

        if(Body)
        {
            PhysicsComponent* Cmp = new PhysicsComponent();
            Cmp->m_Body = Body;
            Cmp->m_EntityID = E->GetID();
            m_Engine->GetSceneGraph()->AddComponent(E,Cmp);
        }
    }
}
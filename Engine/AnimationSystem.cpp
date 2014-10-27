/*
 *  AnimationSystem.cpp
 *
 *  Created by Michael DeCicco on 9/28/14.
 *
 */

#include "AnimationSystem.h"

namespace Reality
{
    Skeleton::Skeleton()
    {
    }
    Skeleton::~Skeleton()
    {
    }
    void Skeleton::AddBone(Bone *B,const string& Name,i16 Parent)
    {
        if(m_BoneMap.Contains(Name))
        {
            Log("Skeleton already contains bone named \"%s\".\n",Name.c_str());
            return;
        }
        m_BoneMap.Insert((i16)m_Bones.size(),Name.c_str());
        m_Bones.push_back(B);
    }
    i16 Skeleton::GetBoneIndex(const string &Name) const
    {
        auto it = m_BoneMap.Find(Name);
        if(it == m_BoneMap.End()) return -1;
        return it->second;
    }
    
    AnimationComponent::AnimationComponent(Skeleton* Skel) : Component(CT_ANIMATION), m_Skeleton(Skel)
    {
    }
    AnimationComponent::~AnimationComponent()
    {
    }
    
    AnimationSystem::AnimationSystem()
    {
    }
    AnimationSystem::~AnimationSystem()
    {
    }

    void AnimationSystem::HandleMessage(const EngineMessage* Msg)
    {
    }

    void AnimationSystem::Initialize()
    {
    }
    void AnimationSystem::Update(Scalar dt)
    {
    }
    void AnimationSystem::Shutdown()
    {
    }

    void AnimationSystem::Serialize()
    {
    }
    void AnimationSystem::Deserialize()
    {
    }
};
/*
 *  AnimationSystem.h
 *
 *  Created by Michael DeCicco on 9/28/14.
 *
 */

#pragma once
#include "EngineSystem.h"
#include "Hashmap.hpp"

namespace Reality
{
    class Skeleton;
    class Bone
    {
        public:
            Bone(Skeleton* Skel) : m_Skeleton(Skel), m_ParentIdx(-1), LocalTransform(Matrix4::Identity), RelativeTransform(Matrix4::Identity), InverseBindPose(Matrix4::Identity) { }
            ~Bone() { }
        
            Matrix4 LocalTransform;
            Matrix4 RelativeTransform;
            Matrix4 InverseBindPose;
        
        protected:
            friend class Mesh;
            i16 m_ParentIdx;
            vector<i16> m_Children;
            Skeleton* m_Skeleton;
    };
    
    class Skeleton
    {
        public:
            Skeleton();
            ~Skeleton();
        
            void AddBone(Bone* B,const string& Name,i16 Parent = -1);
            i16 GetBoneIndex(const string& Name) const;
            Bone* GetBone(i16 Idx) const { return m_Bones[Idx]; }
            Bone* GetBone(const string& Name) const { i16 Idx = GetBoneIndex(Name); if(Idx == -1) { return 0; } return m_Bones[Idx]; }
        
            i16 GetBoneCount() const { return m_Bones.size(); }
            
        
        protected:
            Hashmap<i16>  m_BoneMap;
            vector<Bone*> m_Bones;
    };
    
    class AnimationComponent : public Component
    {
        public:
            AnimationComponent(Skeleton* Skel);
            ~AnimationComponent();
        
        protected:
            Skeleton *m_Skeleton;
    };
    
    class AnimationSystem : public EngineSystem
    {
        public:
            AnimationSystem();
            ~AnimationSystem();
        
            virtual void HandleMessage(const EngineMessage* Msg);
        
            virtual void Initialize();
            virtual void Update(Scalar dt);
            virtual void Shutdown();
        
            virtual void Serialize();
            virtual void Deserialize();
    };
};
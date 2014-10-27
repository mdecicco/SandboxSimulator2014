/*
 *  RenderingAlgorithm.h
 *
 *  Created by Michael DeCicco on 9/9/14.
 *
 */

#pragma once
#include "CullingAlgorithm.h"

namespace Reality
{
    class RenderSystem;
    class RenderingAlgorithm
    {
        public:
            RenderingAlgorithm() { }
            virtual ~RenderingAlgorithm() { }
        
            virtual const char* GetAlgorithmName() const = 0;
            /* Returns number of entities rendered */
            virtual i32 Render(RenderSystem* rSys,RenderList& List) = 0;
    };
    
    class ForwardRenderingAlgorithm : public RenderingAlgorithm
    {
        public:
            ForwardRenderingAlgorithm() { }
            ~ForwardRenderingAlgorithm() { }
        
            virtual const char* GetAlgorithmName() const { return "Forward"; }
            virtual i32 Render(RenderSystem* rSys,RenderList& List);
    };
};
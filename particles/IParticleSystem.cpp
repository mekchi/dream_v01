//
//  CParticleSystem.cpp
//  dream
//
//  Created by Yermek Garifullanov on 11/18/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#include "IParticleSystem.h"

#include "CPS_Star01.h"

CreateParticleSystemFunction IParticleSystem::m_aoCreateFuncs[NUM_PARTICLE_SYSTEM_TYPE];
//DestroyParticleSystemFunction IParticleSystem::m_aoDestroyFuncs[NUM_PARTICLE_SYSTEM_TYPE];

bool IParticleSystem::InitParticleSystems()
{
    m_aoCreateFuncs[PARTICLE_STAR01] = CPS_Star01::Create;

    
    return true;
}

void IParticleSystem::DeinitParticleSystems()
{
    
}

IParticleSystem* IParticleSystem::CreateParticleSystem(E_PARTICLE_SYSTEM_TYPE type)
{
    return m_aoCreateFuncs[type]();
}

void IParticleSystem::DestroyPartickeSystem(IParticleSystem *ParticleSystem)
{
    ParticleSystem->Deinit();
    
    delete ParticleSystem;
}
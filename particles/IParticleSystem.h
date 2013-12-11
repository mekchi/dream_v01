//
//  CParticleSystem.h
//  dream
//
//  Created by Yermek Garifullanov on 11/18/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef __dream__IParticleSystem__
#define __dream__IParticleSystem__

#include <vector>

enum E_PARTICLE_SYSTEM_TYPE
{
    PARTICLE_STAR01 = 0,
    NUM_PARTICLE_SYSTEM_TYPE
};

class IParticleSystem;

typedef IParticleSystem* (*CreateParticleSystemFunction)();
//typedef void (*DestroyParticleSystemFunction)(IParticleSystem *ParticleSystem);

class IParticleSystem
{
    
public:
    
    IParticleSystem() {}
    virtual ~IParticleSystem() {}

    virtual bool Init(void* Property) = 0;
    virtual void Deinit() = 0;
    virtual void Reset(void* Property) = 0;
    virtual bool Update(float DeltaTime) = 0;
    virtual void Render() = 0;
    
    static bool InitParticleSystems();
    static void DeinitParticleSystems();
    static IParticleSystem* CreateParticleSystem(E_PARTICLE_SYSTEM_TYPE type);
    static void DestroyPartickeSystem(IParticleSystem *ParticleSystem);
    
    
private:
    
    static CreateParticleSystemFunction m_aoCreateFuncs[NUM_PARTICLE_SYSTEM_TYPE];
//    static DestroyParticleSystemFunction m_aoDestroyFuncs[NUM_PARTICLE_SYSTEM_TYPE];
    
    
};


#endif /* defined(__dream__CParticleSystem__) */

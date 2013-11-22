//
//  CParticleSystem.h
//  dream
//
//  Created by Yermek Garifullanov on 11/18/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef __dream__IParticleSystem__
#define __dream__IParticleSystem__

class IParticleSystem
{
    
public:
    
    IParticleSystem() {}
    virtual ~IParticleSystem() {}

    virtual bool Init(void* Property) = 0;
    virtual void Deinit() = 0;
    virtual void Update(float DeltaTime) = 0;
    virtual void Render() = 0;
    
protected:
    
    
    
};


#endif /* defined(__dream__CParticleSystem__) */

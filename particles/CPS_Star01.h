//
//  CPS_Star01.h
//  dream
//
//  Created by Yermek Garifullanov on 11/18/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef __dream__CPS_Star01__
#define __dream__CPS_Star01__

#include "IParticleSystem.h"
#include "Graphics.h"
#include "VQMMath.h"

class CPS_Star01 : public IParticleSystem
{
    
public:
    
    CPS_Star01();
    virtual ~CPS_Star01();
    
    virtual bool Init(void* Property = nullptr);
    virtual void Deinit();
    virtual void Reset(void* Property = nullptr);
    virtual bool Update(float DeltaTime);
    virtual void Render();
    
    static IParticleSystem* Create();
//    static void Destroy(IParticleSystem *ParticleSystem);
    
private:
    
    GLuint m_gVAO;
    GLuint m_gVBO[2];
    GLuint m_gTexture;//[2];
    
//    float m_afQuad[30];

    vector3 m_avec3Position[4];
    vector2 m_avec2UV[4];
    
//    int m_iFPS;
    int m_iFrame;
    
    float m_fZoom;

//    float m_fEnergy;
//    vector3 m_fPosition;
};

#endif /* defined(__dream__CPS_Star01__) */

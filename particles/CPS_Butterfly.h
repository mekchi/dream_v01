//
//  CPS_Butterfly.h
//  dream
//
//  Created by Yermek Garifullanov on 12/10/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef __dream__CPS_Butterfly__
#define __dream__CPS_Butterfly__

#include "IParticleSystem.h"
#include "Graphics.h"
#include "VQMMath.h"

class CPS_Butterfly : public IParticleSystem
{
    
public:
    
    CPS_Butterfly();
    virtual ~CPS_Butterfly();
    
    virtual bool Init(void* Property = nullptr);
    virtual void Deinit();
    virtual void Reset(void* Property);
    virtual bool Update(float DeltaTime);
    virtual void Render();
    
    static IParticleSystem* Create();
    //    static void Destroy(IParticleSystem *ParticleSystem);
    
private:
    
    GLuint m_gVAO;
    GLuint m_gVBO[2];
    GLuint m_gTexture;//[2];
    
    //    float m_afQuad[30];
    
    vector3 m_avecQuadPosition[4];
    vector2 m_avecQuadUV[4];
    
    //    int m_iFPS;
    int m_iFrame;
    
    float m_fZoom;
    
    float m_fSpeed;
    vector3 m_avecPosition[13];
    vector3 m_avecVelocity[13];
    vector3 m_avecRotation[13];
    float m_afLifeTime[13];
    bool m_abEnable[13];
    vector3 m_vecColor;
    
    //    float m_fEnergy;
    //    vector3 m_fPosition;
};


#endif /* defined(__dream__CPS_Butterfly__) */

//
//  ILevel.h
//  dream
//
//  Created by Yermek Garifullanov on 12/2/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef __dream__ILevel__
#define __dream__ILevel__

//#include <vector>

#include "IComponent.h"
#include "Graphics.h"
#include "VQMMath.h"
#include "IParticleSystem.h"
#include "CPS_Butterfly.h"

class ILevel : public IComponent
{
    
public:
    
    ILevel() {}
    virtual ~ILevel() {}
    
    virtual E_FAMILY_COMPONENT_ID GetFamilyComponentId() { return FCI_LEVEL; }
    
};

class CLevelRandom : public ILevel
{
    
public:
    
    CLevelRandom();
    virtual ~CLevelRandom();
    
    virtual bool Init(void* Property);
    virtual void Deinit();
    
    virtual E_MESSAGE_RESULT HandleMessage(const CMessage& Message);
    
    virtual E_COMPONENT_ID GetComponentId() { return CI_RANDOM_LEVEL; }
    
    static void Register();
    static IComponent* Create();
    static void Destroy(IComponent* Component);
    
private:
    
    void Update(float DeltaTime);
    void Render();
    void Touch(vector3 *Point);
    void ProtagonistTouch();
    
    bool m_bRender;
    bool m_bRenderPS;
    
    int m_iNumFaceVertex;
    GLuint m_gVAO;
    GLuint m_gVBO;
    
    int m_iNumberColor;
    vector3 *m_avecColor;
    int m_iCurrentColor;
    int *m_aiBoxType;
    E_PARTICLE_SYSTEM_TYPE *m_aeBoxAnimationType;
//    float *m_afBoxTiming;
    bool *m_abBoxActive;
    
    vector3 m_vecPosition;
    
    CPS_Butterfly m_oButterfly;
    
    /* protagonist */
    
    vector3 m_vecProtagonist;
    E_PROTAGONIST_STATUS m_eProtagonistStatus;
    float m_fRadius;
    
    /* wave (revealed area) */
    
    float m_fWaveRadius;
};

#endif /* defined(__dream__ILevel__) */

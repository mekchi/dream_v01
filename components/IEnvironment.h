//
//  IEnvironment.h
//  dream
//
//  Created by Yermek Garifullanov on 11/22/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef __dream__IEnvironment__
#define __dream__IEnvironment__

#include "IComponent.h"

#include "Graphics.h"
#include "DataTypes.h"

//! only one environment can be created

class IEnvironment : public IComponent
{
    
public:
    
    IEnvironment() {}
    virtual ~IEnvironment() {}
    
    virtual E_FAMILY_COMPONENT_ID GetFamilyComponentId() { return FCI_ENVIRONMENT; }
    
};

class CEnvironmentHF : public IEnvironment
{
    
public:
    
    CEnvironmentHF();
    virtual ~CEnvironmentHF();
    
    virtual bool Init(void* Property);
    virtual void Deinit();
    
    virtual E_MESSAGE_RESULT HandleMessage(const CMessage& Message);
    
    virtual E_COMPONENT_ID GetComponentId() { return CI_ENVIRONMENT_HEIGHTFIELD; }
    
    static void Register();
    static IComponent* Create();
    static void Destroy(IComponent* Component);
    
private:
    
    void Update(float DeltaTime);
    void UpdatePhysics(float DeltaTime);
    void UpdateGraphics(float DeltaTime);
    
    void UpdateTouch(float DeltaTime);
    
    void Render();
    void Touch(vector3 *Point);
    void GetNormal(vector3* normal, vector3* p1, vector3* p2, vector3 *p3);
    void ComputeForce(SObjectData *Data);
    
    /* heightfield */
    
    int m_iNumberWidth; // number of heights/points along x axis
    int m_iNumberHeight; // number of heights/points along y axis
    int m_iTotalNumber;
    
    // origin point of the field
    float m_fOriginX;
    float m_fOriginY;
    float m_fLevel; // z coordinate
    
    float m_fStep; // square grid step

    vector3* m_avecPosition;
    float *m_afHeightBuffer;
    float* m_afVelocity;
    float* m_afVelocityContinuous;
    
    /* protagonist */
    
    float m_fRadius; // radius of the protagonist
    
    
    /* graphics */

    int m_iTotalNumberIndex; // number of indices
    int* m_aiIndex; // array of indices
    
    vector3* m_avecNormal;
    
    vector3 m_vecProtagonist; // protagonist position

    GLuint m_gVAO;
    GLuint m_gVBO[3];
    
    /* touch */
    
    float m_fBorderLeft;
    float m_fBorderRight;
    float m_fBorderTop;
    float m_fBorderBottom;

    float m_fWaveRadius;
    float m_fRadiusRatio; // ratio of radius decrease rate
    E_PROTAGONIST_STATUS m_eStatusProtagonist;
    E_ENVIRONMENT_STATUS m_eStatus;
    
    vector3 m_vecCameraDirection;

    vector3 m_vecTouchDirection;
    float m_fTouchLength;
    float m_fTouchDamping;
};

#endif /* defined(__dream__IEnvironment__) */

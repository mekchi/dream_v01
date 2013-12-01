//
//  IProtagonist.h
//  dream
//
//  Created by Yermek Garifullanov on 11/22/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef __dream__IProtagonist__
#define __dream__IProtagonist__

#include "IComponent.h"
#include "Graphics.h"
#include "VQMMath.h"

class IProtagonist : public IComponent
{
    
public:
    
    IProtagonist() {}
    virtual ~IProtagonist() {}
    
    virtual E_FAMILY_COMPONENT_ID GetFamilyComponentId() { return FCI_PROTAGONIST; }
    
};


class CProtagonistSphere : public IProtagonist
{
    
public:
    
    CProtagonistSphere();
    virtual ~CProtagonistSphere();
    
    virtual bool Init(void* Property);
    virtual void Deinit();
    
    virtual E_COMPONENT_ID GetComponentId() { return CI_PROTAGONIST_SPHERE; }
    
    virtual E_MESSAGE_RESULT HandleMessage(const CMessage& Message);
    
    static void Register();
    static IComponent* Create();
    static void Destroy(IComponent* Component);
    
private:
    
    void Update(float DeltaTime);
    void LinearIntegration(float DeltaTime);
    void AngularIntegration(float DeltaTime);
    
    void Render();
    void Touch(vector3* Point);
    
    /* graphics */

    GLuint m_gVAOQuad;
    GLuint m_gVBOQuad;
    GLuint m_gFB;
    GLuint m_gRB;
    GLuint m_gTexture[2];
    GLfloat m_gTextureWidth;
    GLfloat m_gTextureHeight;

    GLuint m_gProtagonistTexture;
    GLuint m_gVAO;
    GLuint m_gVBO;
    GLuint m_gIndices;
    
    
    int m_iNumFaceVertex;
    
    /* physics */
    
    float m_fRadius;
    float m_fMass;
    
    vector3 m_vecForce;
    vector3 m_vecTorque;
    vector3 m_vecPull;
    vector3 m_vecLinearVelocity;
    vector3 m_vecAngularVelocity;
    float m_fLinearDamping;
    float m_fAngularDamping;
    
    /* touch */
    E_PROTAGONIST_STATUS m_eStatus;
    
};

#endif /* defined(__dream__IProtagonist__) */

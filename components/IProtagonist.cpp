//
//  IProtagonist.cpp
//  dream
//
//  Created by Yermek Garifullanov on 11/22/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#include "IProtagonist.h"

#include <assert.h>

#include "Log.h"
#include "Globals.h"
#include "CObjectManager.h"
#include "CMessage.h"
#include "CLoaderMek.h"
#include "LoadTools.h"
#include "CShader.h"

CProtagonistSphere::CProtagonistSphere()
{
    m_eStatus = PRTGS_IDLE;

    v3SetZero(&m_vecForce);
    v3SetZero(&m_vecTorque);
    v3SetZero(&m_vecPull);
    v3SetZero(&m_vecLinearVelocity);
    v3SetZero(&m_vecAngularVelocity);
}

CProtagonistSphere::~CProtagonistSphere()
{
    
}

bool CProtagonistSphere::Init(void* Property)
{
    /* physics */
    m_fRadius = 1.0f;
    m_fMass = 500.0f;
    m_fLinearDamping = 0.9f;
    m_fAngularDamping = 0.9f;
    
    
    /* graphics */
    CLoaderMek sphere;
    unsigned char *data = nullptr;
    unsigned int texW = 0, texH = 0;
    
    if (!sphere.Load("pSphere1.mek") || !LoadPngBytes("protagonist.png", &data, &texW, &texH))
    {
        __LOG("Faild to init (protagonist)");
        
        return false;
    }
    
    m_iNumFaceVertex = sphere.GetNumberOfFace() * 3;
    
    glGenTextures(1, &m_gTexture);

    glBindTexture(GL_TEXTURE_2D, m_gTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenVertexArraysOES(1, &m_gVAO);
    glBindVertexArrayOES(m_gVAO);
    
    glGenBuffers(1, &m_gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO);
    glBufferData(GL_ARRAY_BUFFER, sphere.GetSize(), sphere.GetData(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(SAL_POSITION);
    glVertexAttribPointer(SAL_POSITION, 3, GL_FLOAT, GL_FALSE, 32, 0);
    //    glDisableVertexAttribArray(SAL_POSITION);
    
    glEnableVertexAttribArray(SAL_NORMAL);
    glVertexAttribPointer(SAL_NORMAL, 3, GL_FLOAT, GL_FALSE, 32, (char*)NULL + 12);
    //    glDisableVertexAttribArray(SAL_NORMAL);
    
    glEnableVertexAttribArray(SAL_TEXTURE_COORD);
    glVertexAttribPointer(SAL_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, 32, (char*)NULL + 24);
    //    glDisableVertexAttribArray(SAL_TEXTURE_COORD);
    
    glBindVertexArrayOES(0);
    
    return true;
}

void CProtagonistSphere::Deinit()
{
    
}

E_MESSAGE_RESULT CProtagonistSphere::HandleMessage(const CMessage &Message)
{
    switch (Message.m_eType)
    {
        case MT_UPDATE:
        {
            Update(*(static_cast<float*>(Message.m_vpData)));
            
            return MR_SUCCESS;
        }
            
        case MT_RENDER:
        {
            Render();
            
            return MR_SUCCESS;
        }
            
        case MT_TOUCHED_START:
        {
            Touch(static_cast<vector3*>(Message.m_vpData));
            
            return MR_SUCCESS;
        }
            
        case MT_PROTAGONIST_CHANGE_POSITION:
        {
            m_opObject->SetPosition(static_cast<vector3*>(Message.m_vpData));
            
            return MR_SUCCESS;
        }
    }
    
    return MR_IGNORE;
}

void CProtagonistSphere::Register()
{
    Globals::GetObjectManager().RegisterComponent(CI_PROTAGONIST_SPHERE, Create, Destroy);
    Globals::GetObjectManager().SubscribeMessage(CI_PROTAGONIST_SPHERE, MT_UPDATE);
    Globals::GetObjectManager().SubscribeMessage(CI_PROTAGONIST_SPHERE, MT_RENDER);
    Globals::GetObjectManager().SubscribeMessage(CI_PROTAGONIST_SPHERE, MT_TOUCHED_START);
    Globals::GetObjectManager().SubscribeMessage(CI_PROTAGONIST_SPHERE, MT_PROTAGONIST_CHANGE_POSITION);
}

IComponent* CProtagonistSphere::Create()
{
    return new CProtagonistSphere();
}

void CProtagonistSphere::Destroy(IComponent *Component)
{
    assert(Component);
    delete Component;
}

void CProtagonistSphere::Update(float DeltaTime)
{
    SObjectData data;
    
    m_opObject->GetPosition(&(data.position));
    data.Radius = m_fRadius;
    data.Force = &m_vecForce;
    data.Torque = &m_vecTorque;
    
    // compute forces
    Globals::GetObjectManager().BroadcastMessage(CMessage(MT_PROTAGONIST_COMPUTE_FORCE,
                                                          static_cast<void*>(&data)));
    v3Add(&m_vecForce, &m_vecPull);
    
    LinearIntegration(DeltaTime);
    AngularIntegration(DeltaTime);
    
    v3SetZero(&m_vecForce);
    v3SetZero(&m_vecTorque);
    v3SetZero(&m_vecPull);
    
    switch (m_eStatus)
    {
        case PRTGS_IDLE:
        {
            break;
        }
        case PRTGS_MOVE:
        {

        }
        case PRTGS_SHOW:
        {
            
            break;
        }
    }
}

// direct linear integration
void CProtagonistSphere::LinearIntegration(float DeltaTime)
{
    vector3 dp, temp, a;
    
    v3DivideToR(&a, &m_vecForce, m_fMass);
    v3Add(&a, Globals::GetGravity());

    
    v3Copy(&dp, &m_vecLinearVelocity);
    v3Multiply(&dp, DeltaTime);
    v3MultiplyToR(&temp, &a, DeltaTime * DeltaTime * 0.5f);
    v3Add(&dp, &temp);
    
    
    // new position
    m_opObject->GetPosition(&temp);
    v3Add(&dp, &temp);
    
//    if (m_fRadius + dp.z < 0.0f)
//    {
//        dp.z = 0.0f;
//        m_opObject->SetPosition(&dp);
//    }
//    else
        m_opObject->SetPosition(&dp);
    
    // new velocity
    v3MultiplyToR(&temp, &a, DeltaTime);
    v3Add(&m_vecLinearVelocity, &temp);
    v3Multiply(&m_vecLinearVelocity, m_fLinearDamping);
}

// direct angular integration
void CProtagonistSphere::AngularIntegration(float DeltaTime)
{
//    const float inertia = 0.4f * m_fRadius * m_fRadius;
//    const float invert_inertia = 1.0f / inertia;
    
    //    vector3 v, cross, acceleration;
    //    quaternion qNew, *qOld = m_opObject->GetRotation();
    
    vector3 dv, da, acceleration;
    
    
//    v3MultiplyToR(&acceleration, &m_vecTorque, invert_inertia);
    v3MultiplyToR(&acceleration, &m_vecTorque, 20.0f);
    
    v3Copy(&dv, &m_vecAngularVelocity);
    v3Multiply(&dv, DeltaTime);
    v3MultiplyToR(&da, &acceleration, DeltaTime * DeltaTime * 0.5f);
    v3Add(&dv, &da);
    
    // new rotaion
    v3Add(&dv, m_opObject->GetRotation());
    m_opObject->SetRotation(&dv);
    
    // new velocity
    v3MultiplyToR(&da, &acceleration, DeltaTime);
    v3Add(&m_vecAngularVelocity, &da);
    v3Multiply(&m_vecAngularVelocity, m_fAngularDamping);
}

void CProtagonistSphere::Render()
{
    matrix44 temp, invertTemp;
    
    mMultiplyToR(Globals::GetModelviewMatrix(), m_opObject->GetTransformationMatrix(), &temp);
    mInvertToR(&temp, &invertTemp);
    mTranspose(&invertTemp);
    
    glUseProgram(CShader::GetShader(ST_TEXTURE)->GetProgram());
    
    glUniformMatrix4fv(CShader::GetShader(ST_TEXTURE)->GetUnivormLocation(SUL_MODELVIEW), 1, GL_FALSE, temp.m);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gTexture);
    
    glUniform1i(CShader::GetShader(ST_TEXTURE)->GetUnivormLocation(SUL_TEXTURE0), 0);
    
    glBindVertexArrayOES(m_gVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_iNumFaceVertex);
}

void CProtagonistSphere::Touch(vector3* Point)
{
    v3Set(&m_vecPull, Point->x, Point->y, -0.01f);

    if (v3Magnitude(&m_vecPull) <= m_fRadius)
    {
        m_eStatus = PRTGS_SHOW;

    }
    else
    {
        m_eStatus = PRTGS_MOVE;
        
        vector3 position;
        
        m_opObject->GetPosition(&position);

        v3Subtract(&m_vecPull, &position);
        v3Normalize(&m_vecPull);
        v3Multiply(&m_vecPull, 100000.0f);
    }
    
    Globals::GetObjectManager().BroadcastMessage(CMessage(MT_PROTAGONIST_STATUS, static_cast<void*>(&m_eStatus)));

}

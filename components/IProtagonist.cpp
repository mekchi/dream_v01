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
    m_fRadius = 2.0f;
    m_fMass = 3000.0f;
    m_fLinearDamping = 0.9f;
    m_fAngularDamping = 0.9f;
    
    
    /* graphics */
    CLoaderMek sphere;
    unsigned char *data = nullptr;
    unsigned int texW = 0, texH = 0;
    
//    if (!sphere.Load("pSphere1.mek") || !LoadPngBytes("protagonist.png", &data, &texW, &texH))
    if (!sphere.Load("protagonist.mek") || !LoadPngBytes("protagonist00.png", &data, &texW, &texH))
    {
        __LOG("Faild to init (protagonist)");
        
        return false;
    }
    
    m_iNumFaceVertex = sphere.GetNumberOfFace() * 3;
    
    glGenTextures(1, &m_gProtagonistTexture);

    glBindTexture(GL_TEXTURE_2D, m_gProtagonistTexture);
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
    
    // frame buffer for glow effect
    
    const float quad[30] =
    {
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
        1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f,   0.0f, 1.0f
    };
    
    glGenVertexArraysOES(1, &m_gVAOQuad);
    glBindVertexArrayOES(m_gVAOQuad);
    
    glGenBuffers(1, &m_gVBOQuad);
    glBindBuffer(GL_ARRAY_BUFFER, m_gVBOQuad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(SAL_POSITION);
    glVertexAttribPointer(SAL_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
    glEnableVertexAttribArray(SAL_TEXTURE_COORD);
    glVertexAttribPointer(SAL_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)NULL + (sizeof(float) * 3));
    
    glBindVertexArrayOES(0);
    
    GLint viewport[4];
    
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    m_gTextureWidth = viewport[2];
    m_gTextureHeight = viewport[3];
    
    glGenFramebuffers(1, &m_gFB);
    glGenRenderbuffers(1, &m_gRB);
    glGenTextures(2, m_gTexture);
    
    glBindTexture(GL_TEXTURE_2D, m_gTexture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_gTextureWidth, m_gTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    
    glBindTexture(GL_TEXTURE_2D, m_gTexture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_gTextureWidth, m_gTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glBindRenderbuffer(GL_RENDERBUFFER, m_gRB);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, m_gTextureWidth, m_gTextureHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_gFB);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_gRB);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        __LOG("Faild to create frame buffer");
        
        return false;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, Globals::GetDefaultFramebuffer());
    
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
            
        case MT_ENVIRONMENT_STATUS:
        {
            if (*(static_cast<E_ENVIRONMENT_STATUS*>(Message.m_vpData)) == ENVS_WAVE_DONE)
            {
                m_eStatus = PRTGS_IDLE;
                
                Globals::GetObjectManager().BroadcastMessage(CMessage(MT_PROTAGONIST_STATUS, static_cast<void*>(&m_eStatus)));
            }
            
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
    Globals::GetObjectManager().SubscribeMessage(CI_PROTAGONIST_SPHERE, MT_ENVIRONMENT_STATUS);
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
    if (m_eStatus == PRTGS_IDLE || m_eStatus == PRTGS_MOVE)
    {
        SObjectData data;
        vector3 position;
        
        m_opObject->GetPosition(&(data.position));
        data.Radius = m_fRadius;
        data.Force = &m_vecForce;
        data.Torque = &m_vecTorque;
        
        // compute forces
        Globals::GetObjectManager().BroadcastMessage(CMessage(MT_PROTAGONIST_COMPUTE_FORCE, static_cast<void*>(&data)));
        v3Add(&m_vecForce, &m_vecPull);
        
        LinearIntegration(DeltaTime);
        AngularIntegration(DeltaTime);
        
        m_opObject->GetPosition(&position);
        Globals::GetObjectManager().BroadcastMessage(CMessage(MT_PROTAGONIST_POSITION,         static_cast<void*>(&position)));
        
        v3SetZero(&m_vecForce);
        v3SetZero(&m_vecTorque);
        v3SetZero(&m_vecPull);
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
    
//    glEnable(GL_CULL_FACE);
    
//    glCullFace(GL_FRONT);
    
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    glUseProgram(CShader::GetShader(ST_TEXTURE)->GetProgram());
    
    glUniformMatrix4fv(CShader::GetShader(ST_TEXTURE)->GetUnivormLocation(SUL_MODELVIEW), 1, GL_FALSE, temp.m);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gProtagonistTexture);
    
    glUniform1i(CShader::GetShader(ST_TEXTURE)->GetUnivormLocation(SUL_TEXTURE0), 0);
    
    glBindVertexArrayOES(m_gVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_iNumFaceVertex);
    
//    glDisable(GL_BLEND);
//    
//    glDisable(GL_CULL_FACE);
    
    
    
    // basic rendering
//    
//    glBindFramebuffer(GL_FRAMEBUFFER, m_gFB);
//    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//    glClear(GL_COLOR_BUFFER_BIT);
//    
//    glUseProgram(CShader::GetShader(ST_TEXTURE)->GetProgram());
//
//    glUniformMatrix4fv(CShader::GetShader(ST_TEXTURE)->GetUnivormLocation(SUL_MODELVIEW), 1, GL_FALSE, temp.m);
//
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, m_gProtagonistTexture);
//
//    glUniform1i(CShader::GetShader(ST_TEXTURE)->GetUnivormLocation(SUL_TEXTURE0), 0);
//
//    glBindVertexArrayOES(m_gVAO);
//    glDrawArrays(GL_TRIANGLES, 0, m_iNumFaceVertex);
//    
//    glBindTexture(GL_TEXTURE_2D, m_gTexture[0]);
//    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, m_gTextureWidth, m_gTextureHeight, 0);
//    
//    // vertical blur
//    
//    glClear(GL_COLOR_BUFFER_BIT);
//    
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, m_gTexture[0]);
//    
//    glUseProgram(CShader::GetShader(ST_VERTICAL_GLOW)->GetProgram());
//    glUniform1i(CShader::GetShader(ST_VERTICAL_GLOW)->GetUnivormLocation(SUL_TEXTURE0), 0);
//    
//    glBindVertexArrayOES(m_gVAOQuad);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    
//    glBindTexture(GL_TEXTURE_2D, m_gTexture[1]);
//    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, m_gTextureWidth, m_gTextureHeight, 0);
//    
//    // horizontal blur
//    
//    glClear(GL_COLOR_BUFFER_BIT);
//    
//    glActiveTexture(GL_TEXTURE0 + 1);
//    glBindTexture(GL_TEXTURE_2D, m_gTexture[1]);
//    
//    glUseProgram(CShader::GetShader(ST_HORIZONTAL_GLOW)->GetProgram());
//    glUniform1i(CShader::GetShader(ST_HORIZONTAL_GLOW)->GetUnivormLocation(SUL_TEXTURE0), 1);
//    
//    glBindVertexArrayOES(m_gVAOQuad);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    
//    glBindTexture(GL_TEXTURE_2D, m_gTexture[0]);
//    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, m_gTextureWidth, m_gTextureHeight, 0);
//    
//    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    
//    glUseProgram(CShader::GetShader(ST_BLEND)->GetProgram());
//    
//    glUniform1i(CShader::GetShader(ST_BLEND)->GetUnivormLocation(SUL_TEXTURE0), 1);
//    glUniform1i(CShader::GetShader(ST_BLEND)->GetUnivormLocation(SUL_TEXTURE1), 2);
//    
//    glBindVertexArrayOES(m_gVAOQuad);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    
//    glBindTexture(GL_TEXTURE_2D, m_gTexture[0]);
//    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, m_gTextureWidth, m_gTextureHeight, 0);
//
//    glBindFramebuffer(GL_FRAMEBUFFER, Globals::GetDefaultFramebuffer());
//    
//    glUseProgram(CShader::GetShader(ST_TEXTURE)->GetProgram());
//    
//    glUniformMatrix4fv(CShader::GetShader(ST_TEXTURE)->GetUnivormLocation(SUL_MODELVIEW), 1, GL_FALSE, temp.m);
//    
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, m_gProtagonistTexture);
//    
//    glUniform1i(CShader::GetShader(ST_TEXTURE)->GetUnivormLocation(SUL_TEXTURE0), 0);
//    
//    glBindVertexArrayOES(m_gVAO);
//    glDrawArrays(GL_TRIANGLES, 0, m_iNumFaceVertex);
//    
//    glBindTexture(GL_TEXTURE_2D, m_gTexture[0]);
//    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, m_gTextureWidth, m_gTextureHeight, 0);
//    
//    glBindTexture(GL_TEXTURE_2D, m_gTexture[1]);
//    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, m_gTextureWidth, m_gTextureHeight, 0);
//    
//    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    
//    glUseProgram(CShader::GetShader(ST_BLEND)->GetProgram());
//    
//    glUniform1i(CShader::GetShader(ST_BLEND)->GetUnivormLocation(SUL_TEXTURE0), 1);
//    glUniform1i(CShader::GetShader(ST_BLEND)->GetUnivormLocation(SUL_TEXTURE1), 2);
//    
//    glBindVertexArrayOES(m_gVAOQuad);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
    
//    glBindTexture(GL_TEXTURE_2D, 0);
}

void CProtagonistSphere::Touch(vector3* Point)
{
    if (m_eStatus != PRTGS_TOUCH)
    {
        vector3 position;

        v3Copy(&m_vecPull, Point);
        m_opObject->GetPosition(&position);
        m_vecPull.z = position.z;
        v3Subtract(&m_vecPull, &position);

        if (v3Magnitude(&m_vecPull) <= m_fRadius)
        {
            m_eStatus = PRTGS_TOUCH;
            
            m_opObject->ResetRotaiton();
        }
        else
        {
            //v3Set(&m_vecPull, Point->x, Point->y, -0.01f);
            
            m_eStatus = PRTGS_MOVE;

            v3Normalize(&m_vecPull);
            v3Multiply(&m_vecPull, 150000.0f);
        }

        Globals::GetObjectManager().BroadcastMessage(CMessage(MT_PROTAGONIST_STATUS, static_cast<void*>(&m_eStatus)));
    }
}

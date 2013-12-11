//
//  ILevel.cpp
//  dream
//
//  Created by Yermek Garifullanov on 12/2/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#include "ILevel.h"

#include <stdlib.h>
#include <assert.h>

#include "Log.h"
#include "CMessage.h"
#include "Globals.h"
#include "CShader.h"
#include "CObjectManager.h"
#include "CLoaderMek.h"

CLevelRandom::CLevelRandom()
{
    v3SetZero(&m_vecProtagonist);
    m_bRender = false;
    m_bRenderPS = false;
}

CLevelRandom::~CLevelRandom()
{
    
}

bool CLevelRandom::Init(void *Property)
{
    if (!Property)
    {
        __LOG("Property is empty");
        
        return false;
    }

    SGameProperty *prop = static_cast<SGameProperty*>(Property);
    vector3 position;
    vector3 color;
    
    m_opObject->GetPosition(&position);
    v3Set(&color, 0.0f, 0.0f, 0.0f);
    
    SPS_Property psProp = {0, &position, 20.0f, &color};
    
    if (!m_oButterfly.Init(static_cast<void*>(&psProp)))
    {
        __LOG("Faild to init particle system");
        
        return false;
    }
    
    CLoaderMek cube;
    
    if (!cube.Load("pCube1.mek"))
        //    if (!sphere.Load("pPlane1.mek"))
    {
        __LOG("Faild to load 3d model file");
        
        return false;
    }
    
    m_iNumFaceVertex = cube.GetNumberOfFace() * 3;
    
    glGenVertexArraysOES(1, &m_gVAO);
    glBindVertexArrayOES(m_gVAO);
    
    glGenBuffers(1, &m_gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO);
    glBufferData(GL_ARRAY_BUFFER, cube.GetSize(), static_cast<GLvoid*>(cube.GetData()), GL_STATIC_DRAW);
    
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
    
    m_fRadius = prop->ProtagonistRadius;
    m_fWaveRadius = prop->WaveRadius;
    
    m_iNumberColor = 3;
    m_avecColor = new vector3[m_iNumberColor];
    m_aiBoxType = new int[m_iNumberColor];
//    m_afBoxTiming = new float[n];
    m_aeBoxAnimationType = new E_PARTICLE_SYSTEM_TYPE[m_iNumberColor];
    m_abBoxActive = new bool[m_iNumberColor];

    
    for (int i = 0; i < m_iNumberColor; i++)
    {
        m_abBoxActive[i] = true;
//        m_afBoxTiming = 
    }
    
    v3Set(&m_avecColor[0], 1.0f, 0.0f, 0.0f);
    v3Set(&m_avecColor[1], 0.0f, 1.0f, 0.0f);
    v3Set(&m_avecColor[2], 0.0f, 0.0f, 1.0f);

    return true;
}

void CLevelRandom::Deinit()
{
    if (m_aiBoxType) delete m_aiBoxType;
//    if (m_afBoxTiming) delete m_afBoxTiming;
    if (m_aeBoxAnimationType) delete m_aeBoxAnimationType;
    if (m_abBoxActive) delete m_abBoxActive;
    
    glDeleteBuffers(1, &m_gVBO);
    glDeleteVertexArraysOES(1, &m_gVAO);
}

E_MESSAGE_RESULT CLevelRandom::HandleMessage(const CMessage &Message)
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
            
        case MT_PROTAGONIST_STATUS:
        {
//            m_eProtagonistStatus = ;
            
            if (*(static_cast<E_PROTAGONIST_STATUS*>(Message.m_vpData)) == PRTGS_TOUCH)
            {
                ProtagonistTouch();
            }
            else
            {
                m_bRender = false;
            }
            
            return MR_SUCCESS;
        }
            
        case MT_PROTAGONIST_POSITION:
        {
            v3Copy(&m_vecProtagonist, static_cast<vector3*>(Message.m_vpData));
            
            return MR_SUCCESS;
        }
    
    }
    
    return MR_IGNORE;
}

void CLevelRandom::Register()
{
    Globals::GetObjectManager().RegisterComponent(CI_RANDOM_LEVEL, Create, Destroy);
    Globals::GetObjectManager().SubscribeMessage(CI_RANDOM_LEVEL, MT_UPDATE);
    Globals::GetObjectManager().SubscribeMessage(CI_RANDOM_LEVEL, MT_RENDER);
    Globals::GetObjectManager().SubscribeMessage(CI_RANDOM_LEVEL, MT_TOUCHED_START);
    Globals::GetObjectManager().SubscribeMessage(CI_RANDOM_LEVEL, MT_PROTAGONIST_STATUS);
    Globals::GetObjectManager().SubscribeMessage(CI_RANDOM_LEVEL, MT_PROTAGONIST_POSITION);
}

IComponent* CLevelRandom::Create()
{
    return new CLevelRandom();
}

void CLevelRandom::Destroy(IComponent *Component)
{
    assert(Component);
    delete Component;
}

void CLevelRandom::Update(float DeltaTime)
{
    if (m_bRenderPS)
    {
        m_bRenderPS = m_oButterfly.Update(DeltaTime);
    }
}

void CLevelRandom::Render()
{
    if (m_bRender)
    {
        matrix44 temp, invertTemp, result;

        mSetIdentity(&temp);
        mSetTranslation(&temp, m_vecPosition.x, m_vecPosition.y, m_vecPosition.z);
        mMultiplyToR(Globals::GetModelviewMatrix(), &temp, &result);
        mInvertToR(&result, &invertTemp);
        mTranspose(&invertTemp);

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        
        glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
        
        glUseProgram(CShader::GetShader(ST_DEFAULT)->GetProgram());

        glUniformMatrix4fv(CShader::GetShader(ST_DEFAULT)->GetUnivormLocation(SUL_MODELVIEW), 1, GL_FALSE, result.m);
        glUniformMatrix4fv(CShader::GetShader(ST_DEFAULT)->GetUnivormLocation(SUL_NORMALMATRIX), 1, GL_FALSE, invertTemp.m);
        glUniform3fv(CShader::GetShader(ST_DEFAULT)->GetUnivormLocation(SUL_LIGHTPOSITION), 1, m_vecProtagonist.v);
        glUniform3fv(CShader::GetShader(ST_DEFAULT)->GetUnivormLocation(SUL_COLOR0), 1, m_avecColor[m_iCurrentColor].v);
        

        glBindVertexArrayOES(m_gVAO);
        glDrawArrays(GL_TRIANGLES, 0, m_iNumFaceVertex);

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }
    
    if (m_bRenderPS)
    {
        m_oButterfly.Render();
    }
    
}

void CLevelRandom::Touch(vector3 *Point)
{
    if (m_bRender)
    {
        vector3 dist;

        v3Copy(&dist, Point);
        dist.z = m_vecPosition.z;

        v3Subtract(&dist, &m_vecPosition);

        if (v3Magnitude(&dist) <= 3.0f)
        {
            vector3 color;

            v3Set(&color, 0.0f, 1.0f, 0.0f);

            SPS_Property prop = {0, &m_vecProtagonist, 20.0f, &m_avecColor[m_iCurrentColor]};

            m_oButterfly.Reset(static_cast<void*>(&prop));

            m_abBoxActive[m_iCurrentColor] = false;
            
            m_bRender = false;
            m_bRenderPS = true;
        }
    }
}

void CLevelRandom::ProtagonistTouch()
{
    m_iCurrentColor = rand() % m_iNumberColor;
    
    if (m_abBoxActive[m_iCurrentColor])
    {
        float ratio = (float)(rand() % 90 + 40) * 0.01;

        m_bRender = true;
        v3Set(&m_vecPosition, m_vecProtagonist.x + (m_fWaveRadius * ratio),
              (m_vecProtagonist.y + (m_fWaveRadius * ratio)),
              -6.0f);
    }
}

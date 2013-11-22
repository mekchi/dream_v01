//
//  IMiracleBox.cpp
//  dream
//
//  Created by Yermek Garifullanov on 11/18/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#include "IMiracleBox.h"

#include <assert.h>

#include "Globals.h"
#include "CObjectManager.h"
#include "CSoundManager.h"
#include "CMessage.h"
#include "CShader.h"
#include "Log.h"
#include "CLoaderMek.h"


// ---- CMiracleBox00 ----

bool CMiracleBox00::Init(void *Property)
{
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
    return true;
}

void CMiracleBox00::Deinit()
{
    glDeleteBuffers(1, &m_gVBO);
    glDeleteVertexArraysOES(1, &m_gVAO);
}

E_MESSAGE_RESULT CMiracleBox00::HandleMessage(const CMessage &Message)
{
    switch (Message.m_eType)
    {
        case MT_RENDER:
        {
            Render();
            
            return MR_SUCCESS;
        }
        case MT_PROTAGONIST_POSITION:
        {
            m_vecProtagonist = static_cast<vector3*>(Message.m_vpData);
            
            return MR_SUCCESS;
        }
            
        case MT_TOUCHED_START:
        {
            Globals::GetSoundManager().PlaySound(0);
            
            return MR_SUCCESS;
        }
    }
    
    return MR_IGNORE;
}


void CMiracleBox00::Register()
{
    Globals::GetObjectManager().RegisterComponent(CI_MIRACLE_BOX_00, Create, Destroy);
    Globals::GetObjectManager().SubscribeMessage(CI_MIRACLE_BOX_00, MT_RENDER);
    Globals::GetObjectManager().SubscribeMessage(CI_MIRACLE_BOX_00, MT_PROTAGONIST_POSITION);
    Globals::GetObjectManager().SubscribeMessage(CI_MIRACLE_BOX_00, MT_TOUCHED_START);
}

IComponent* CMiracleBox00::Create()
{
    return new CMiracleBox00();
}

void CMiracleBox00::Destroy(IComponent *Component)
{
    assert(Component);
    delete Component;
}

void CMiracleBox00::Render()
{
    
    matrix44 invertTemp;
    
    mInvertToR(Globals::GetModelviewMatrix(), &invertTemp);
    
    glUseProgram(CShader::GetShader(ST_DEFAULT)->GetProgram());
    
    glUniformMatrix4fv(CShader::GetShader(ST_DEFAULT)->GetUnivormLocation(SUL_MODELVIEW), 1, GL_FALSE, (Globals::GetModelviewMatrix())->m);
    glUniformMatrix4fv(CShader::GetShader(ST_DEFAULT)->GetUnivormLocation(SUL_NORMALMATRIX), 1, GL_FALSE, invertTemp.m);
    glUniform3fv(CShader::GetShader(ST_DEFAULT)->GetUnivormLocation(SUL_LIGHTPOSITION), 1, m_vecProtagonist->v);
    
    glBindVertexArrayOES(m_gVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_iNumFaceVertex);
}


// ---- CMiracleBox01 ----

bool CMiracleBox01::Init(void *Property)
{
    SPS_Spark01_Property prop = {0, m_opObject->GetPosition()};
    
    m_oSpark.Init(static_cast<void*>(&prop));
    
    return true;
}

void CMiracleBox01::Deinit()
{
    m_oSpark.Deinit();
}

E_MESSAGE_RESULT CMiracleBox01::HandleMessage(const CMessage &Message)
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
    }
    
    return MR_IGNORE;
}


void CMiracleBox01::Register()
{
    Globals::GetObjectManager().RegisterComponent(CI_MIRACLE_BOX_01, Create, Destroy);
    Globals::GetObjectManager().SubscribeMessage(CI_MIRACLE_BOX_01, MT_UPDATE);
    Globals::GetObjectManager().SubscribeMessage(CI_MIRACLE_BOX_01, MT_RENDER);
}

IComponent* CMiracleBox01::Create()
{
    return new CMiracleBox01();
}

void CMiracleBox01::Destroy(IComponent *Component)
{
    assert(Component);
    delete Component;
}

void CMiracleBox01::Update(float DeltaTime)
{
    m_oSpark.Update(DeltaTime);
}

void CMiracleBox01::Render()
{
    m_oSpark.Render();
}

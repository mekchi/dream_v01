//
//  CPS_Spark01.cpp
//  dream
//
//  Created by Yermek Garifullanov on 11/18/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#include "CPS_Spark01.h"

#include <stdlib.h>

#include "Log.h"
#include "LoadTools.h"
#include "lodepng.h"
#include "CShader.h"

CPS_Spark01::CPS_Spark01()
{
    
}

CPS_Spark01::~CPS_Spark01()
{
    
}

bool CPS_Spark01::Init(void* Property)
{
    unsigned char* data = nullptr;
    unsigned char* image = nullptr;
    int size = 0;
    unsigned int w, h;
    
    if (Property == nullptr)
    {
        __LOG("Faild to init CPS_Spark01 (null property)");
        
        return false;
    }
    
    if (!LoadFileBytes("flash.png", &data, &size))
    {
        __LOG("Faild to init CPS_Spark01");
        
        return false;
    }
    
    if (0 != lodepng_decode32(&image, &w, &h, data, size))
    {
        __LOG("Faild to init CPS_Spark01 (texture problem)");
        
        delete data;
        
        return false;
    }
    
    SPS_Spark01_Property *prop = static_cast<SPS_Spark01_Property*>(Property);
    vector3 *p = prop->position;
    
//    m_iFPS = prop->fps;
    m_iFrame = 0;
    
    v3Set(&m_avec3Position[0], p->x - 5.5f, p->y - 5.5f, p->z);
    v3Set(&m_avec3Position[1], p->x + 5.5f, p->y - 5.5f, p->z);
    v3Set(&m_avec3Position[2], p->x - 5.5f, p->y + 5.5f, p->z);
    v3Set(&m_avec3Position[3], p->x + 5.5f, p->y + 5.5f, p->z);
    
    m_avec2UV[0].x = 0.0f;
    m_avec2UV[0].y = 0.0f;
    
    m_avec2UV[1].x = m_avec2UV[0].x + 0.5f;
    m_avec2UV[1].y = m_avec2UV[0].y;
    
    m_avec2UV[2].x = m_avec2UV[0].x;
    m_avec2UV[2].y = m_avec2UV[0].y + 0.5f;
    
    m_avec2UV[3].x = m_avec2UV[0].x + 0.5f;
    m_avec2UV[3].y = m_avec2UV[0].y + 0.5f;
    
    glGenTextures(1, &m_gTexture);
    glBindTexture(GL_TEXTURE_2D, m_gTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glGenVertexArraysOES(1, &m_gVAO);
    glBindVertexArrayOES(m_gVAO);

    glGenBuffers(2, m_gVBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector3) * 6, m_avec3Position, GL_STATIC_DRAW);
    glEnableVertexAttribArray(SAL_POSITION);
    glVertexAttribPointer(SAL_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(vector3), 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector2) * 6, m_avec2UV, GL_STATIC_DRAW);
    glEnableVertexAttribArray(SAL_TEXTURE_COORD);
    glVertexAttribPointer(SAL_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(vector2), 0);
    
    glBindVertexArrayOES(0);
    
    delete data;
    delete image;
    
    return true;
}

void CPS_Spark01::Deinit()
{
    if (m_gTexture) glDeleteTextures(1, &m_gTexture);
    if (m_gVAO) glDeleteVertexArraysOES(1, &m_gVAO);
    if (m_gVBO) glDeleteBuffers(2, m_gVBO);
}

void CPS_Spark01::Update(float DeltaTime)
{
    m_avec2UV[0].x = 0.0f;
    m_avec2UV[0].y = 0.0f;
    
    if (m_iFrame == 1)
    {
        m_avec2UV[0].x = 0.5f;
        m_avec2UV[0].y = 0.0f;
    }
    if (m_iFrame == 2)
    {
        m_avec2UV[0].x = 0.0f;
        m_avec2UV[0].y = 0.5f;
    }
    if (m_iFrame == 3)
    {
        m_avec2UV[0].x = 0.5f;
        m_avec2UV[0].y = 0.5f;
        
        m_iFrame = -1;
    }
    
    m_avec2UV[1].x = m_avec2UV[0].x + 0.5f;
    m_avec2UV[1].y = m_avec2UV[0].y;

    m_avec2UV[2].x = m_avec2UV[0].x;
    m_avec2UV[2].y = m_avec2UV[0].y + 0.5f;

    m_avec2UV[3].x = m_avec2UV[0].x + 0.5f;
    m_avec2UV[3].y = m_avec2UV[0].y + 0.5f;
    
    m_iFrame++;
    
    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector2) * 6, m_avec2UV, GL_STATIC_DRAW);
}

void CPS_Spark01::Render()
{
    CShader *shader = CShader::GetShader(ST_PARTICLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glUseProgram(shader->GetProgram());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gTexture);
    glUniform1i(shader->GetUnivormLocation(SUL_TEXTURE0), 0);

    glBindVertexArrayOES(m_gVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glDisable(GL_BLEND);
}

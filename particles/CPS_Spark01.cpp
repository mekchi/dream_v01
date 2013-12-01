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
    m_fZoom = 0.0f;
}

CPS_Spark01::~CPS_Spark01()
{
    
}

bool CPS_Spark01::Init(void* Property)
{
    unsigned char *sprite = nullptr;
    unsigned int w, h;
    
    if (Property == nullptr)
    {
        __LOG("Faild to init CPS_Spark01 (null property)");
        
        return false;
    }

    if (!LoadPngBytes("s_star.png", &sprite, &w, &h))
    {
        __LOG("Faild to init CPS_Spark01 (texture problem)");
        
        return false;
    }
    
    
    SPS_Spark01_Property *prop = static_cast<SPS_Spark01_Property*>(Property);
    vector3 *p = prop->position;
    
//    m_iFPS = prop->fps;
    m_iFrame = 0;
    
    m_fZoom = 2.0f;
    
    v3Set(&m_avec3Position[0], p->x - m_fZoom, p->y - m_fZoom, p->z);
    v3Set(&m_avec3Position[1], p->x + m_fZoom, p->y - m_fZoom, p->z);
    v3Set(&m_avec3Position[2], p->x - m_fZoom, p->y + m_fZoom, p->z);
    v3Set(&m_avec3Position[3], p->x + m_fZoom, p->y + m_fZoom, p->z);
    
    m_avec2UV[0].x = 0.0f;
    m_avec2UV[0].y = 0.0f;
    
    m_avec2UV[1].x = m_avec2UV[0].x + 0.3333f;
    m_avec2UV[1].y = m_avec2UV[0].y;
    
    m_avec2UV[2].x = m_avec2UV[0].x;
    m_avec2UV[2].y = m_avec2UV[0].y + 0.5f;
    
    m_avec2UV[3].x = m_avec2UV[0].x + 0.3333f;
    m_avec2UV[3].y = m_avec2UV[0].y + 0.5f;
    
    glGenTextures(1, &m_gTexture);
    glBindTexture(GL_TEXTURE_2D, m_gTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sprite);
    
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

    DeletePngBytes(sprite);
    
    return true;
}

void CPS_Spark01::Deinit()
{
//    if (m_gTexture) glDeleteTextures(2, m_gTexture);
    if (m_gTexture) glDeleteTextures(1, &m_gTexture);
    if (m_gVAO) glDeleteVertexArraysOES(1, &m_gVAO);
    if (m_gVBO) glDeleteBuffers(2, m_gVBO);
}

void CPS_Spark01::Update(float DeltaTime)
{
    const float u = 1.0f / 3.0f;
    const float v = 0.5f;
    const float fps = 0.3f / 6.0f;

    static float time = 0.0f;
    static int frame = 0;
    static float x = 0.0f, y = 0.0f;
    
    if (time >= fps)
    {
        frame++;
        
        time = 0.0f;
        if (frame == 6)
        {
            frame = 0;
            x = 0.0f;
            y = 0.0f;
        }
        else if (frame == 3)
                {
                    y = 1.0f;
                    x = 0.0f;
                }
            else
                x += 1.0f;
        
        m_avec2UV[0].x = x * u;
        m_avec2UV[0].y = y * v;
        
        m_avec2UV[1].x = m_avec2UV[0].x + u;
        m_avec2UV[1].y = m_avec2UV[0].y;
        
        m_avec2UV[2].x = m_avec2UV[0].x;
        m_avec2UV[2].y = m_avec2UV[0].y + v;
        
        m_avec2UV[3].x = m_avec2UV[0].x + u;
        m_avec2UV[3].y = m_avec2UV[0].y + v;
        
        glBindBuffer(GL_ARRAY_BUFFER, m_gVBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vector2) * 6, m_avec2UV, GL_STATIC_DRAW);
    }
        
    time += DeltaTime;
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

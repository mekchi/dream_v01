//
//  CPS_Butterfly.cpp
//  dream
//
//  Created by Yermek Garifullanov on 12/10/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#include "CPS_Butterfly.h"

#include <stdlib.h>

#include "Log.h"
#include "LoadTools.h"
#include "lodepng.h"
#include "DataTypes.h"
#include "CShader.h"

CPS_Butterfly::CPS_Butterfly()
{
    m_fZoom = 0.0f;
    m_fSpeed = 0.0f;
}

CPS_Butterfly::~CPS_Butterfly()
{
    
}

bool CPS_Butterfly::Init(void* Property)
{
    unsigned char *sprite = nullptr;
    unsigned int w, h;
    
    if (Property == nullptr)
    {
        __LOG("Faild to init CPS_Star01 (null property)");
        
        return false;
    }
    
    if (!LoadPngBytes("s_bf.png", &sprite, &w, &h))
    {
        __LOG("Faild to init CPS_Star01 (texture problem)");
        
        return false;
    }
    
    
    SPS_Property *prop = static_cast<SPS_Property*>(Property);
    vector3 *p = prop->Position;
    
    v3Copy(&m_vecColor, prop->Color);
    //    m_iFPS = prop->fps;
    m_iFrame = 0;
    
    m_fZoom = 3.0f;
    
    v3Set(&m_avecQuadPosition[0], p->x - m_fZoom, p->y - m_fZoom, p->z);
    v3Set(&m_avecQuadPosition[1], p->x + m_fZoom, p->y - m_fZoom, p->z);
    v3Set(&m_avecQuadPosition[2], p->x - m_fZoom, p->y + m_fZoom, p->z);
    v3Set(&m_avecQuadPosition[3], p->x + m_fZoom, p->y + m_fZoom, p->z);
    
    m_avecQuadUV[0].x = 0.0f;
    m_avecQuadUV[0].y = 0.0f;
    
    m_avecQuadUV[1].x = m_avecQuadUV[0].x + 0.3333f;
    m_avecQuadUV[1].y = m_avecQuadUV[0].y;
    
    m_avecQuadUV[2].x = m_avecQuadUV[0].x;
    m_avecQuadUV[2].y = m_avecQuadUV[0].y + 0.5f;
    
    m_avecQuadUV[3].x = m_avecQuadUV[0].x + 0.3333f;
    m_avecQuadUV[3].y = m_avecQuadUV[0].y + 0.5f;
    
    Reset(Property);
    
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector3) * 4, m_avecQuadPosition, GL_STATIC_DRAW);
    glEnableVertexAttribArray(SAL_POSITION);
    glVertexAttribPointer(SAL_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(vector3), 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector2) * 4, m_avecQuadUV, GL_STATIC_DRAW);
    glEnableVertexAttribArray(SAL_TEXTURE_COORD);
    glVertexAttribPointer(SAL_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(vector2), 0);
    
    glBindVertexArrayOES(0);
    
    DeletePngBytes(sprite);
    
    return true;
}

void CPS_Butterfly::Deinit()
{
    //    if (m_gTexture) glDeleteTextures(2, m_gTexture);
    if (m_gTexture) glDeleteTextures(1, &m_gTexture);
    if (m_gVAO) glDeleteVertexArraysOES(1, &m_gVAO);
    if (m_gVBO) glDeleteBuffers(2, m_gVBO);
}

void CPS_Butterfly::Reset(void* Property)
{
    float speed, angle = 0.0f;
    SPS_Property *prop = static_cast<SPS_Property*>(Property);
    vector3 *p = prop->Position;
    
    v3Copy(&m_vecColor, prop->Color);
    
    for (int i = 0; i < 13; i++)
    {
        m_afLifeTime[i] = prop->LifeTime;
        v3Set(&m_avecPosition[i], p->x, p->y, -25.0f);
        
        speed = (float)(rand() % 100) * 0.2f;
        v3Set(&m_avecVelocity[i], cosf((angle) * 0.01745f) * speed,
              sinf((angle) * 0.01745f) * speed,
              (float)i / 100.0f);

        m_abEnable[i] = true;
        
        v3Set(&m_avecRotation[i], cosf((90.0f + angle) * 0.01745f), sinf((90.0f + angle) * 0.01745f), 0.0f);
        angle += 20.0f;
    }
}

bool CPS_Butterfly::Update(float DeltaTime)
{
    const float u = 1.0f / 3.0f;
    const float v = 0.5f;
    const float fps = 0.3f / 6.0f;
    
    static float time = 0.0f;
    static int frame = 0;
    static float x = 0.0f, y = 0.0f;
    
    vector3 velocity;
    int i, inactive = 0;
    
    for (i = 0; i < 13; i++)
    {
        if (m_abEnable[i])
        {
            m_afLifeTime[i] -= DeltaTime;
            if (m_afLifeTime[i] > 0.0f)
            {
                v3MultiplyToR(&velocity, &m_avecVelocity[i], DeltaTime);
                v3Add(&m_avecPosition[i], &velocity);
            }
            else
            {
                m_abEnable[i] = false;
            }
        }
        else
            inactive++;
    }
    
    if (inactive == i)
    {
        return false;
    }
    
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
        
        m_avecQuadUV[0].x = x * u;
        m_avecQuadUV[0].y = y * v;
        
        m_avecQuadUV[1].x = m_avecQuadUV[0].x + u;
        m_avecQuadUV[1].y = m_avecQuadUV[0].y;
        
        m_avecQuadUV[2].x = m_avecQuadUV[0].x;
        m_avecQuadUV[2].y = m_avecQuadUV[0].y + v;
        
        m_avecQuadUV[3].x = m_avecQuadUV[0].x + u;
        m_avecQuadUV[3].y = m_avecQuadUV[0].y + v;
        
        glBindBuffer(GL_ARRAY_BUFFER, m_gVBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vector2) * 6, m_avecQuadUV, GL_STATIC_DRAW);
    }
    
    time += DeltaTime;
    
    return true;
}

void CPS_Butterfly::Render()
{
    vector3 tempPosition[4];
    CShader *shader = CShader::GetShader(ST_PARTICLE);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    glUseProgram(shader->GetProgram());
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gTexture);
    glUniform1i(shader->GetUnivormLocation(SUL_TEXTURE0), 0);
    
    glUniform3fv(shader->GetUnivormLocation(SUL_COLOR0), 1, m_vecColor.v);
    
    glBindVertexArrayOES(m_gVAO);
    
    for (int i = 0; i < 13; i++)
    {
        if (m_abEnable[i])
        {
            float x = m_fZoom * m_avecRotation[i].x;
            float y = m_fZoom * m_avecRotation[i].y;
            
//            v3Set(&m_avecQuadPosition[0], p->x - m_fZoom, p->y - m_fZoom, p->z);
//            v3Set(&m_avecQuadPosition[1], p->x + m_fZoom, p->y - m_fZoom, p->z);
//            v3Set(&m_avecQuadPosition[2], p->x - m_fZoom, p->y + m_fZoom, p->z);
//            v3Set(&m_avecQuadPosition[3], p->x + m_fZoom, p->y + m_fZoom, p->z);
            
//            v3Set(&tempPosition[0], m_avecPosition[i].x - m_fZoom,
//                  m_avecPosition[i].y - m_fZoom,
//                  m_avecPosition[i].z);
//            
//            v3Set(&tempPosition[1], m_avecPosition[i].x + m_fZoom,
//                  m_avecPosition[i].y - m_fZoom,
//                  m_avecPosition[i].z);
//            
//            v3Set(&tempPosition[2], m_avecPosition[i].x - m_fZoom,
//                  m_avecPosition[i].y + m_fZoom,
//                  m_avecPosition[i].z);
//            
//            v3Set(&tempPosition[3], m_avecPosition[i].x + m_fZoom,
//                  m_avecPosition[i].y + m_fZoom,
//                  m_avecPosition[i].z);

            
            v3Set(&tempPosition[0], m_avecPosition[i].x - x + y,
                  m_avecPosition[i].y - y - x,
                  m_avecPosition[i].z);
            
            v3Set(&tempPosition[1], m_avecPosition[i].x + x + y,
                  m_avecPosition[i].y + y - x,
                  m_avecPosition[i].z);
            
            v3Set(&tempPosition[2], m_avecPosition[i].x - x - y,
                  m_avecPosition[i].y - y + x,
                  m_avecPosition[i].z);
            
            v3Set(&tempPosition[3], m_avecPosition[i].x + x - y,
                  m_avecPosition[i].y + y + x,
                  m_avecPosition[i].z);
            
            glBindBuffer(GL_ARRAY_BUFFER, m_gVBO[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vector3) * 4, tempPosition, GL_STATIC_DRAW);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }
    
    glDisable(GL_BLEND);
    
}

IParticleSystem* CPS_Butterfly::Create()
{
    return new CPS_Butterfly();
}
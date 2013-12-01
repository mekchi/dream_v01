//
//  IEnvironment.cpp
//  dream
//
//  Created by Yermek Garifullanov on 11/22/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#include "IEnvironment.h"

#include <assert.h>
#include <math.h>

#include "Log.h"
#include "Globals.h"
#include "CMessage.h"
#include "CShader.h"


CEnvironmentHF::CEnvironmentHF()
{
    m_eStatusProtagonist = PRTGS_IDLE;
    m_eStatus = ENVS_IDLE;
}

CEnvironmentHF::~CEnvironmentHF()
{
}

bool CEnvironmentHF::Init(void* Property)
{
    assert(Property);
    if (!Property) return false;
    
    SHeightFieldProperty *property = static_cast<SHeightFieldProperty*>(Property);
    
    m_fStep = property->GridStep;
    m_iNumberWidth = property->NumberWidthHeight * 2 + 2;
    m_iNumberHeight = property->NumberHeightHeight * 2 + 2;
    m_iTotalNumber = m_iNumberWidth * m_iNumberHeight;
    m_fRadius = property->ProtagonistRadius;
    m_fOriginX = property->OriginX * 2 - m_fStep;
    m_fOriginY = property->OriginY * 2 - m_fStep;
    m_fLevel = property->Level;

    // define border
    m_fBorderLeft = property->Left;
    m_fBorderRight = property->Right;
    m_fBorderTop = property->Top;
    m_fBorderBottom = property->Bottom;
    
    m_fRadiusRatio = 1.99f;
    m_fTouchDamping = 0.9f;
    
    m_avecPosition = new vector3[m_iTotalNumber];
    m_afHeightBuffer = new float[m_iTotalNumber];
    m_afVelocity = new float[m_iTotalNumber];
    m_afVelocityContinuous = new float[m_iTotalNumber];
    
    float angle = m_fStep * 0.393f;
    float x, y, z;
    int id;
    
    
    for (int i = 0; i < m_iNumberWidth; i++)
    {
        x = m_fOriginX + ((float)i) * m_fStep;
        
        for (int j = 0; j < m_iNumberHeight; j++)
        {
            id = i * m_iNumberHeight + j;
            y = m_fOriginY + ((float)j) * m_fStep;
            
            if (i % 2 == 0)
                z = 0.5f * cosf(angle * (float)j);
            else
                z = 0.5f * cosf((angle + 1.57f) * (float)j);

            m_afVelocityContinuous[id] = 0.0f;
//            m_afVelocityContinuous[id] = z;
//            m_afVelocityContinuous[id] = 0.09f - ((float)(rand() % 900) / 10000.0f);
            
            v3Set(&m_avecPosition[id], x, y, m_fLevel + m_afVelocityContinuous[id]);
            
            m_afVelocity[id] = 0.0f;

        }
    }
    
    SMaterialData data = {"wave.bmp"};
    
    m_iTotalNumberIndex = (m_iNumberWidth - 1)  * (m_iNumberHeight - 1) * 6;
    
    m_aiIndex = new int[m_iTotalNumberIndex];
    m_avecNormal = new vector3[m_iTotalNumber];
    
    int k = 0;
    
    for (int i = 0; i < m_iTotalNumber; i++)
    {
        v3Set(&m_avecNormal[i], 0.0f, 0.0f, 1.0f);
    }
    
    for (int i = 0; i < m_iNumberWidth - 1; i++)
        for (int j = 0; j < m_iNumberHeight - 1; j++)
        {
            id = i * m_iNumberHeight + j;
            
//            if ((i + j) % 2 == 0)
//            {
                m_aiIndex[k++] = id;
                m_aiIndex[k++] = id + m_iNumberHeight;
                m_aiIndex[k++] = id + m_iNumberHeight + 1;
                
                m_aiIndex[k++] = id;
                m_aiIndex[k++] = id + m_iNumberHeight + 1;
                m_aiIndex[k++] = id + 1;
//            }
//            else
//            {
//                m_aiIndex[k++] = id;
//                m_aiIndex[k++] = id + m_iNumberHeight;
//                m_aiIndex[k++] = id + 1;
//                
//                m_aiIndex[k++] = id + m_iNumberHeight;
//                m_aiIndex[k++] = id + m_iNumberHeight + 1;
//                m_aiIndex[k++] = id + 1;
//            }
        }
    
    glGenVertexArraysOES(1, &m_gVAO);
    glBindVertexArrayOES(m_gVAO);
    
    glGenBuffers(3, m_gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector3) * m_iTotalNumber, static_cast<void*>(m_avecPosition), GL_STATIC_DRAW);
    glEnableVertexAttribArray(SAL_POSITION);
    glVertexAttribPointer(SAL_POSITION, 3, GL_FLOAT, GL_FALSE, 12, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector3) * m_iTotalNumber, static_cast<void*>(m_avecNormal), GL_STATIC_DRAW);
    glEnableVertexAttribArray(SAL_NORMAL);
    glVertexAttribPointer(SAL_NORMAL, 3, GL_FLOAT, GL_FALSE, 12, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gVBO[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_iTotalNumberIndex, static_cast<void*>(m_aiIndex), GL_STATIC_DRAW);
    
    glBindVertexArrayOES(0);
    
    //    glGenTextures(1, &m_gTexture);
    //
    //    glBindTexture(GL_TEXTURE_2D, m_gTexture);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //
    //    if (!loadBmpImage(Data->TextureFileName.c_str()))
    //        return false;

    
    return true;
}

void CEnvironmentHF::Deinit()
{
    if (m_avecPosition) delete m_avecPosition;
    if (m_afHeightBuffer) delete m_afHeightBuffer;
    if (m_afVelocity) delete m_afVelocity;
    if (m_afVelocityContinuous) delete m_afVelocityContinuous;
    if (m_aiIndex) delete m_aiIndex;
    if (m_avecNormal) delete m_avecNormal;
}

E_MESSAGE_RESULT CEnvironmentHF::HandleMessage(const CMessage &Message)
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
            m_eStatusProtagonist = *(static_cast<E_PROTAGONIST_STATUS*>(Message.m_vpData));
            
            return MR_SUCCESS;
        }
            
        case MT_PROTAGONIST_COMPUTE_FORCE:
        {
            ComputeForce(static_cast<SObjectData*>(Message.m_vpData));
            
            return MR_SUCCESS;
        }
    }
    
    return MR_IGNORE;
}

void CEnvironmentHF::Register()
{
    Globals::GetObjectManager().RegisterComponent(CI_ENVIRONMENT_HEIGHTFIELD, Create, Destroy);
    Globals::GetObjectManager().SubscribeMessage(CI_ENVIRONMENT_HEIGHTFIELD, MT_UPDATE);
    Globals::GetObjectManager().SubscribeMessage(CI_ENVIRONMENT_HEIGHTFIELD, MT_RENDER);
    Globals::GetObjectManager().SubscribeMessage(CI_ENVIRONMENT_HEIGHTFIELD, MT_TOUCHED_START);
    Globals::GetObjectManager().SubscribeMessage(CI_ENVIRONMENT_HEIGHTFIELD, MT_PROTAGONIST_STATUS);
    Globals::GetObjectManager().SubscribeMessage(CI_ENVIRONMENT_HEIGHTFIELD, MT_PROTAGONIST_COMPUTE_FORCE);
}

IComponent* CEnvironmentHF::Create()
{
    return new CEnvironmentHF();
}

void CEnvironmentHF::Destroy(IComponent *Component)
{
    assert(Component);
    delete Component;
}

void CEnvironmentHF::Update(float DeltaTime)
{
    UpdatePhysics(DeltaTime);
    UpdateGraphics(DeltaTime);
}

void CEnvironmentHF::UpdatePhysics(float DeltaTime)
{
    const float speed = 3.0f;
    const float ratio = (speed * speed) / (m_fStep * m_fStep);
    const int lastX = (m_iNumberWidth - 1) * m_iNumberHeight;
    const int beforeLastX = (m_iNumberWidth - 2) * m_iNumberHeight;
    const float maxSlope = 5.0f;
    int ij = 0;
    float offset = 0.0f;
    float maxOffset = 0.0f;
    
    if (m_eStatusProtagonist == PRTGS_MOVE)
        UpdateTouch(DeltaTime);

    if (m_eStatus == ENVS_IDLE)
    {
        for (int i = 1; i < m_iNumberWidth - 1; i++)
            for (int j = 1; j < m_iNumberHeight - 1; j++)
            {
                ij = i * m_iNumberHeight + j;
                
                m_afVelocity[ij] += (m_avecPosition[ij - m_iNumberHeight].z +
                                     m_avecPosition[ij + m_iNumberHeight].z +
                                     m_avecPosition[ij - 1].z +
                                     m_avecPosition[ij + 1].z -
                                     4.0f * m_avecPosition[ij].z) * ratio * DeltaTime;
            }
        
        for (int i = 1; i < m_iNumberWidth - 1; i++)
            for (int j = 1; j < m_iNumberHeight - 1; j++)
            {
                ij = i * m_iNumberHeight + j;
                
                m_avecPosition[ij].z += m_afVelocityContinuous[ij] + m_afVelocity[ij] * DeltaTime;
//                m_avecPosition[ij].z += m_afVelocity[ij] * DeltaTime;

                m_afVelocity[ij] *= 0.99;
            }
        
        for (int i = 1; i < m_iNumberWidth - 1; i++)
            for (int j = 1; j < m_iNumberHeight - 1; j++)
            {
                ij = i * m_iNumberHeight + j;
                
                offset = (m_avecPosition[ij - m_iNumberHeight].z +
                          m_avecPosition[ij + m_iNumberHeight].z +
                          m_avecPosition[ij - 1].z +
                          m_avecPosition[ij + 1].z) * 0.25f - m_avecPosition[ij].z;
                
                maxOffset = maxSlope * m_fStep;
                
                if (offset > maxOffset) m_avecPosition[ij].z += offset - maxOffset;
                if (offset < -maxOffset) m_avecPosition[ij].z += offset + maxOffset;
                
            }
        
        for (int i = 1; i < m_iNumberHeight - 1; i++)
        {
            m_avecPosition[i].z *= m_fStep;
            m_avecPosition[i].z += m_avecPosition[m_iNumberHeight + i].z * speed * DeltaTime;
            m_avecPosition[i].z /= m_fStep + speed * DeltaTime;
            
            //        m_avecPosition[i].z = (float)(rand() % 100) / 100.0f;
            
            m_avecPosition[lastX + i].z *= m_fStep;
            m_avecPosition[lastX + i].z += m_avecPosition[beforeLastX + i].z * speed * DeltaTime;
            m_avecPosition[lastX + i].z /= m_fStep + speed * DeltaTime;
        }
        
        for (int i = 1; i < m_iNumberWidth - 1; i++)
        {
            int idf = i * m_iNumberHeight;
            int idl = (i + 1) * m_iNumberHeight - 1;
            
            m_avecPosition[idf].z *= m_fStep;
            m_avecPosition[idf].z += m_avecPosition[idf + 1].z * speed * DeltaTime;
            m_avecPosition[idf].z /= m_fStep + speed * DeltaTime;
            
            m_avecPosition[idl].z *= m_fStep;
            m_avecPosition[idl].z += m_avecPosition[idl - 1].z * speed * DeltaTime;
            m_avecPosition[idl].z /= m_fStep + speed * DeltaTime;
        }
    }
}

void CEnvironmentHF::GetNormal(vector3* normal, vector3* p1, vector3* p2, vector3 *p3)
{
    vector3 v1, v2;
    
    v3SubtractToR(&v1, p2, p1);
    v3SubtractToR(&v2, p3, p1);
    v3CrossProduct(normal, &v1, &v2);
    v3Normalize(normal);
    //if (normal->z < 0.0f) vMultiply(normal, -1.0f);
}

void CEnvironmentHF::UpdateGraphics(float DeltaTime)
{
    int ij = 0;
    vector3 n1, n2, n3, n4, n5, n6;
    
    for (int i = 1; i < m_iNumberWidth - 1; i++)
        for (int j = 1; j < m_iNumberHeight - 1; j++)
        {
            ij = i * m_iNumberHeight + j;
            
            m_avecNormal[ij].x = m_avecPosition[ij - m_iNumberHeight].z - m_avecPosition[ij + m_iNumberHeight].z;
            m_avecNormal[ij].y = m_avecPosition[ij - 1].z - m_avecPosition[ij + 1].z;
            m_avecNormal[ij].z = 1.0f;
            
//            GetNormal(&n1, &m_avecPosition[ij], &m_avecPosition[ij - 1], &m_avecPosition[ij + m_iNumberHeight]);
//            GetNormal(&n2, &m_avecPosition[ij], &m_avecPosition[ij + m_iNumberHeight], &m_avecPosition[ij + m_iNumberHeight + 1]);
//            GetNormal(&n3, &m_avecPosition[ij], &m_avecPosition[ij + m_iNumberHeight + 1], &m_avecPosition[ij + 1]);
//            GetNormal(&n4, &m_avecPosition[ij], &m_avecPosition[ij + 1], &m_avecPosition[ij - m_iNumberHeight]);
//            GetNormal(&n5, &m_avecPosition[ij], &m_avecPosition[ij - m_iNumberHeight], &m_avecPosition[ij -m_iNumberHeight - 1]);
//            GetNormal(&n6, &m_avecPosition[ij], &m_avecPosition[ij - m_iNumberHeight - 1], &m_avecPosition[ij - 1]);
//            
//            v3Add(&n1, &n2);
//            v3Add(&n1, &n3);
//            v3Add(&n1, &n4);
//            v3Add(&n1, &n5);
//            v3Add(&n1, &n6);
//            v3Normalize(&n1);
            
//              v3Copy(&m_avecNormal[ij], &n1);
            v3Normalize(&m_avecNormal[ij]);
        }
    
    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector3) * m_iTotalNumber, static_cast<void*>(m_avecPosition), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector3) * m_iTotalNumber, static_cast<void*>(m_avecNormal), GL_STATIC_DRAW);
}

void CEnvironmentHF::UpdateTouch(float DeltaTime)
{
    const int centerx = m_iNumberWidth / 2;
    const int centery = m_iNumberHeight / 2;
    const int leftx = m_iNumberWidth / 4 + 1;
    const int lefty = m_iNumberHeight / 4 + 1;
//    const int nx = leftx + centerx;
//    const int ny = lefty + centery;
    static int idx = 0, idy = 0;
    
    
    if (m_eStatus == ENVS_IDLE)
    {
        if (m_vecProtagonist.x > m_fBorderRight || m_vecProtagonist.x <m_fBorderLeft
            || m_vecProtagonist.y > m_fBorderTop || m_vecProtagonist.y < m_fBorderBottom)
        {
            idx = (int)((m_vecProtagonist.x - m_fOriginX) / m_fStep);
            idy = (int)((m_vecProtagonist.y - m_fOriginY) / m_fStep);
            
            int index = idx * m_iNumberHeight + idy;
            
            v3Set(&m_vecCameraDirection, m_avecPosition[index].x,
                  m_avecPosition[index].y, 0.0f);
            
            m_eStatus = ENVS_CAMERA_MOVE;
        }
    }
    
    if (m_eStatus == ENVS_CAMERA_MOVE)
    {
        static float t = 0.0f;
        vector3 translate;
        
        t += DeltaTime;

        v3MultiplyToR(&translate, &m_vecCameraDirection, -t);
        Globals::SetTranslation(&translate);
        
        if (t > 1.0f) // one second animation
        {
            // copy heights
            
            for (int i = 0; i < m_iTotalNumber; i++)
            {
//                m_afHeightBuffer[i] = m_avecPosition[i].z;
                m_afVelocity[i] = 0.0f;
            }
            
            // shift heights
            
            int cornerx = idx - leftx + 1;
            int cornery = idy - lefty + 1;
            
            for (int i = 0; i < centerx - 1; i++)
                for (int j = 0; j < centery - 1; j++)
                {
                    m_avecPosition[((leftx + i) * m_iNumberHeight) + lefty + j].z =
                    m_avecPosition[((cornerx + i) * m_iNumberHeight) + cornery + j].z;
                    
                    m_avecPosition[((cornerx + i) * m_iNumberHeight) + cornery + j].z = 0.0f;
                    //m_afHeightBuffer[((cornerx + i) * m_iNumberHeight) + cornery + j];
                }

            
            //t = 1.0f;
            m_eStatus = ENVS_IDLE;
            t = 0.0f;
            //v3SetZero(&translate);
            //Globals::SetTranslation(&translate);
            Globals::ResetModelview();
            
            v3Subtract(&m_vecProtagonist, &m_vecCameraDirection);
            
            Globals::GetObjectManager().BroadcastMessage(CMessage(MT_PROTAGONIST_CHANGE_POSITION, static_cast<void*>(&m_vecProtagonist)));
            
            idx = 0;
            idy = 0;
        }
    }
    
//    v3Multiply(&newPosition, -1.0f);
    
    
//    shiftLength += m_fTouchLength * DeltaTime;
//    v3MultiplyToR(&shift, &m_vecTouchDirection, m_fTouchLength * DeltaTime);
//
//    v3Add(&newPosition, &shift);
//    m_opObject->SetPosition(&newPosition);
//    
//    m_fTouchLength *= m_fTouchDamping;
    
    
//    vector3 vn, vt, v;
//    float t = 0.0f;
//    int ij = 0, x = 0, y = 0;
//    int radiusCells = (int)(m_fRadius / m_fStep);
//    int diameterCells = radiusCells * 2;
//    int startX = 0, startY = 0;
//    
//    v3Copy(&vt, &m_vecTouchDirection);
//    v3Multiply(&vt, m_fRadius);
//    
//    while (m_fTouchLength > v3Magnitude(&vt))
//    {
//        x = (int)((vt.x - m_fOriginX) / m_fStep);
//        y = (int)((vt.y - m_fOriginY) / m_fStep);
////        t = m_fRadius * m_fRadiusRatio;
//        startX = x - radiusCells; // can't be negative
//        startY = y - radiusCells; // can't be negative
//        
//        for (int i = 0; i < diameterCells; i++)
//            for (int j = 0; j < diameterCells; j++)
//            {
//                ij = (startX + i) * m_iNumberHeight + (startY + j);
//                
////                m_avecPosition[ij].z -= t;
//                m_avecPosition[ij].z -= m_fRadius;
//            }
//        v3MultiplyToR(&v, &vn, m_fRadius);
//        v3Add(&vt, &v);
//    }

}

void CEnvironmentHF::Render()
{
    matrix44 temp, invertTemp;
    
    mMultiplyToR(Globals::GetModelviewMatrix(), m_opObject->GetTransformationMatrix(), &temp);
    
    mInvertToR(&temp, &invertTemp);
    mTranspose(&invertTemp);
    
    glUseProgram(CShader::GetShader(ST_HF)->GetProgram());
    
    glUniformMatrix4fv(CShader::GetShader(ST_HF)->GetUnivormLocation(SUL_MODELVIEW), 1, GL_FALSE, temp.m);
    glUniformMatrix4fv(CShader::GetShader(ST_HF)->GetUnivormLocation(SUL_NORMALMATRIX), 1, GL_FALSE, invertTemp.m);
    glUniform3fv(CShader::GetShader(ST_HF)->GetUnivormLocation(SUL_LIGHTPOSITION), 1, m_vecProtagonist.v);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, m_gTexture);
//    glUniform1i(CShader::GetShader(ST_HF)->GetUnivormLocation(SUL_TEXTURE0), 0);
    
    glUniform1f(CShader::GetShader(ST_HF)->GetUnivormLocation(SUL_FLOAT0), m_fWaveRadius);
    
    glBindVertexArrayOES(m_gVAO);
    glDrawElements(GL_TRIANGLES, m_iTotalNumberIndex, GL_UNSIGNED_INT, (void*)0);
}

void CEnvironmentHF::Touch(vector3 *Point)
{

    
    
//    vector3 vn, vt, v;
//    float length = v3Magnitude(Point);
//    float t = 0.0f;
//    int ij = 0, x = 0, y = 0;
//    int radiusCells = (int)(m_fRadius / m_fStep);
//    int diameterCells = radiusCells * 2;
//    int startX = 0, startY = 0;
    
    m_fTouchLength = v3Magnitude(Point);
    v3Copy(&m_vecTouchDirection, Point);
    v3Normalize(&m_vecTouchDirection);
    v3Multiply(&m_vecTouchDirection, -1.0f);
//    v3Copy(&vt, &vn);
//    v3Multiply(&vt, m_fRadius);
//    
//    while (length > v3Magnitude(&vt))
//    {
//        x = (int)((vt.x - m_fOriginX) / m_fStep);
//        y = (int)((vt.y - m_fOriginY) / m_fStep);
//        t = m_fRadius * m_fRadiusRatio;
//        startX = x - radiusCells; // can't be negative
//        startY = y - radiusCells; // can't be negative
//        
//        for (int i = 0; i < diameterCells; i++)
//            for (int j = 0; j < diameterCells; j++)
//            {
//                ij = (startX + i) * m_iNumberHeight + (startY + j);
//                
//                m_avecPosition[ij].z -= t;
//            }
//        v3MultiplyToR(&v, &vn, m_fRadius);
//        v3Add(&vt, &v);
//    }
 
    
//    int x = (int)(Data->x / m_fStep) + 1;
//    int y = (int)(Data->y / m_fStep) + 1;
//    
//    m_avecPosition[x * m_iNumberHeight + y].z -= 2.0f;
}


void CEnvironmentHF::ComputeForce(SObjectData *Data)
{
    vector3 center = Data->position;
    vector3 ft, v, r;
    float radius = Data->Radius;
    float diff = 0.0f, totalDiff = 0.0f, maxDiff = 0.0f;
    int id, idx, idy, n;
    float ax, ay, length2;
    
    v3Copy(&m_vecProtagonist, &center);
    
    // check if lowest point of sphere below the level
    if ((center.z - radius) < m_fLevel)
    {
        // check all points in square radius * 2 x radius * 2
        idx = (int)((center.x - radius - m_fOriginX) / m_fStep);
        idy = (int)((center.y - radius - m_fOriginY) / m_fStep);
        n = 2.0f * radius / m_fStep;
        
        for (int x = 0; x < n; x++)
            for (int y = 0; y < n; y++)
            {
                id = (idx + x) * m_iNumberHeight + (idy + y);
                
                ax = center.x - m_avecPosition[id].x;
                ay = center.y - m_avecPosition[id].y;
                
                length2 = ax * ax + ay * ay;
                
                if (length2 <= radius * radius)
                {
                    float b = fastSqrt(radius * radius - length2);
                    
                    v3Set(&r, ax, ay, b);
                    v3Normalize(&r);
                    v3Multiply(&r, radius);
                    
                    if (center.z - b < m_fLevel)
                    {
                        diff = b - center.z + m_fLevel;
                        totalDiff += diff;
                        m_avecPosition[id].z -= diff;
                        
                        // calculate torque vector
                        if (maxDiff < diff)
                        {
                            maxDiff = diff;
                            v3Copy(&v, &r);
                        }

                    }
                }
            }
        if (totalDiff == 0.0f)
            totalDiff = radius;
        
        // compute and add force
        v3MultiplyToR(&ft, Globals::GetGravity(), -1000.0f * totalDiff * m_fStep * m_fStep);
        v3Add(Data->Force, &ft);
        // add torque
        v3Set(&r, 0.0f, 0.0f, maxDiff);
        v3CrossProduct(&ft, &v, &r);
        v3Add(Data->Torque, &ft);
    }
    
}

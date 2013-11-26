//
//  IDrawObject.cpp
//  dream
//
//  Created by Yermek Garifullanov on 10/7/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#include "IDrawObject.h"

#include <assert.h>

#include "Log.h"
#include "Globals.h"
#include "CObjectManager.h"
#include "CMessage.h"
#include "CLoaderMek.h"
#include "IMaterial.h"
#include "ShaderIds.h"


// ---- CDrawSphere ----

CDrawSphere::CDrawSphere()
{
    
}

CDrawSphere::~CDrawSphere()
{
    
}

bool CDrawSphere::Init(void* Property)
{
    CLoaderMek sphere;
    
    if (!sphere.Load("pSphere1.mek"))
//    if (!sphere.Load("pPlane1.mek"))
    {
        __LOG("Faild to load 3d model file");
        
        return false;
    }
//    sphere.Load("pCube1.mek");
//    sphere.Load("pPlane1.mek");
    
    m_iNumFaceVertex = sphere.GetNumberOfFace() * 3;
    
//    m_opMaterial = new CMaterialDefault(this);
//    m_opMaterial = new CMaterialGlow(this);
    m_opMaterial = new CMaterialTexture(this);
    
    SMaterialData data = {"tex01.bmp"};
    
    if (!m_opMaterial->Init(&data))
    {
        __LOG("Faild to load material");
    }
    
    glGenVertexArraysOES(1, &m_gVAO);
    glBindVertexArrayOES(m_gVAO);
    
    glGenBuffers(1, &m_gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO);
    glBufferData(GL_ARRAY_BUFFER, sphere.GetSize(), static_cast<GLvoid*>(sphere.GetData()), GL_STATIC_DRAW);

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

void CDrawSphere::Deinit()
{
    if (m_opMaterial)
        delete m_opMaterial;
    
    glDeleteBuffers(1, &m_gVBO);
    glDeleteVertexArraysOES(1, &m_gVAO);
}

E_MESSAGE_RESULT CDrawSphere::HandleMessage(const CMessage &Message)
{
    switch(Message.m_eType)
    {
        case MT_RENDER:
        {
            SMaterialRenderData data = {m_opObject->GetTransformationMatrix(), nullptr};

            m_opMaterial->Render(&data);
            
            return MR_SUCCESS;
        }
    }
    
    return MR_IGNORE;
}

void CDrawSphere::Render()
{
    glBindVertexArrayOES(m_gVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_iNumFaceVertex);
}

void CDrawSphere::Register()
{
    Globals::GetObjectManager().RegisterComponent(CI_DRAW_SPHERE, Create, Destroy);
    Globals::GetObjectManager().SubscribeMessage(CI_DRAW_SPHERE, MT_RENDER);
}

IComponent* CDrawSphere::Create()
{
    return new CDrawSphere();
}

void CDrawSphere::Destroy(IComponent *Component)
{
    assert(Component);
    delete Component;
}

// ---- CDrawSurface ----

CDrawSurface::CDrawSurface()
{
    m_vecProtagonist = nullptr;
    m_fWaveRadius = 0.0f;
}

CDrawSurface::~CDrawSurface()
{
    
}

bool CDrawSurface::Init(void *Property)
{
    assert(Property);
    if (!Property) return Property;
    
    // test
//    SMaterialData data = {"wave.bmp"};
//    waveTest = new CMaterialWave();
//    waveTest->Init(&data);
    
    
    // init material
//    m_opMaterial = new CMaterialDefault(this);
    SMaterialData data = {"wave.bmp"};
    
    m_opMaterial = new CMaterialHeightField(this);
    m_opMaterial->Init(&data);
    
    // init draw stuff
    SHeightFieldProperty *property = static_cast<SHeightFieldProperty*>(Property);
    
    m_iNumberWidth = property->NumberWidthHeight + 2;
    m_iNumberHeight = property->NumberHeightHeight + 2;
    m_iTotalNumber = m_iNumberWidth * m_iNumberHeight;
    
    m_iTotalNumberIndex = (m_iNumberWidth - 1)  * (m_iNumberHeight - 1) * 6;
//    m_iTotalNumberIndex = (m_iNumberWidth - 1)  * (m_iNumberHeight - 1) * 8;
    
    m_aiIndex = new int[m_iTotalNumberIndex];
    m_avecNormal = new vector3[m_iTotalNumber];
    
    int id = 0, k = 0;
    
    for (int i = 0; i < m_iTotalNumber; i++)
    {
        v3Set(&m_avecNormal[i], 0.0f, 0.0f, 1.0f);
    }
    
    for (int i = 0; i < m_iNumberWidth - 1; i++)
        for (int j = 0; j < m_iNumberHeight - 1; j++)
        {
            id = i * m_iNumberHeight + j;
            m_aiIndex[k++] = id;
            m_aiIndex[k++] = id + m_iNumberHeight;
            m_aiIndex[k++] = id + m_iNumberHeight + 1;
            
            m_aiIndex[k++] = id;
            m_aiIndex[k++] = id + m_iNumberHeight + 1;
            m_aiIndex[k++] = id + 1;
            
//            if (id % 2 == i % 2)
//            {
//                m_aiIndex[k++] = id;
//                m_aiIndex[k++] = id + m_iNumberHeight;
//                m_aiIndex[k++] = id + m_iNumberHeight + 1;
//                
////                m_aiIndex[k++] = id;
//                
//                m_aiIndex[k++] = id;
//                m_aiIndex[k++] = id + m_iNumberHeight + 1;
//                m_aiIndex[k++] = id + 1;
//                
////                m_aiIndex[k++] = id;
//            }
//            else
//            {
//                m_aiIndex[k++] = id;
//                m_aiIndex[k++] = id + m_iNumberHeight;
//                m_aiIndex[k++] = id + 1;
//                
////                m_aiIndex[k++] = id;
//                
//                m_aiIndex[k++] = id + m_iNumberHeight;
//                m_aiIndex[k++] = id + m_iNumberHeight + 1;
//                m_aiIndex[k++] = id + 1;
//                
////                m_aiIndex[k++] = id + m_iNumberHeight;
//            }
        }
    
//    vSet(&m_avecNormal[925], 0.63f, 0.0f, 0.77f);
//    vSet(&m_avecNormal[975], 0.63f, 0.0f, 0.77f);
//    vSet(&m_avecNormal[825], -0.63f, 0.0f, 0.77f);
//    vSet(&m_avecNormal[874], 0.0, -0.63f, 0.77f);
//    vSet(&m_avecNormal[876], 0.0f, 0.63f, 0.77f);
    
    
    glGenVertexArraysOES(1, &m_gVAO);
    glBindVertexArrayOES(m_gVAO);
    
    glGenBuffers(3, m_gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector3) * m_iTotalNumber, (void*)(0), GL_STATIC_DRAW);
    glEnableVertexAttribArray(SAL_POSITION);
    glVertexAttribPointer(SAL_POSITION, 3, GL_FLOAT, GL_FALSE, 12, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector3) * m_iTotalNumber, static_cast<void*>(m_avecNormal), GL_STATIC_DRAW);
    glEnableVertexAttribArray(SAL_NORMAL);
    glVertexAttribPointer(SAL_NORMAL, 3, GL_FLOAT, GL_FALSE, 12, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gVBO[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_iTotalNumberIndex, static_cast<void*>(m_aiIndex), GL_STATIC_DRAW);

    glBindVertexArrayOES(0);
    
    return true;
}

void CDrawSurface::Deinit()
{
    if (m_avecNormal) delete m_avecNormal;
    if (m_aiIndex) delete m_aiIndex;
    
    if (m_opMaterial)
    {
        m_opMaterial->Deinit();
        delete m_opMaterial;
    }
    
    glDeleteBuffers(3, m_gVBO);
    glDeleteVertexArraysOES(1, &m_gVAO);
}

E_MESSAGE_RESULT CDrawSurface::HandleMessage(const CMessage &Message)
{
    switch(Message.m_eType)
    {
        case MT_UPDATE_RENDER:
        {
            UpdateRenderBuffers(static_cast<vector3*>(Message.m_vpData));
            
            return MR_SUCCESS;
        }
            
        case MT_RENDER:
        {
            //float radius = 0.0f;
            SMaterialRenderData data = {m_opObject->GetTransformationMatrix(), m_vecProtagonist, &m_fWaveRadius, nullptr};
            
            m_opMaterial->Render(&data);
            
//            waveTest->Render();
            
            return MR_SUCCESS;
        }
            
        case MT_PROTAGONIST_POSITION:
        {
            //vCopy(&m_vecProtagonist, static_cast<vector3*>(Message.m_vpData));
            
            m_vecProtagonist = static_cast<vector3*>(Message.m_vpData);
            
            return MR_SUCCESS;
        }
            
        case MT_PROTAGONIST_TOUCH:
        {
            m_fWaveRadius = *(static_cast<float*>(Message.m_vpData));
            
            return MR_SUCCESS;
        }
            
//        case MT_TOUCHED_START:
//        {
//            fileit();
//            
//            return MR_SUCCESS;
//        }
    }
    
    return MR_IGNORE;
}

void CDrawSurface::Register()
{
    Globals::GetObjectManager().RegisterComponent(CI_DRAW_SURFACE, Create, Destroy);
    Globals::GetObjectManager().SubscribeMessage(CI_DRAW_SURFACE, MT_UPDATE_RENDER);
    Globals::GetObjectManager().SubscribeMessage(CI_DRAW_SURFACE, MT_RENDER);
    Globals::GetObjectManager().SubscribeMessage(CI_DRAW_SURFACE, MT_PROTAGONIST_POSITION);
    Globals::GetObjectManager().SubscribeMessage(CI_DRAW_SURFACE, MT_PROTAGONIST_TOUCH);
    
//    Globals::GetObjectManager().SubscribeMessage(CI_DRAW_SURFACE, MT_TOUCHED_START);
}

IComponent* CDrawSurface::Create()
{
    return new CDrawSurface();
}

void CDrawSurface::Destroy(IComponent *Component)
{
    assert(Component);
    delete Component;
}

void CDrawSurface::GetNormal(vector3* normal, vector3* p1, vector3* p2, vector3 *p3)
{
    vector3 v1, v2;
    
    v3SubtractToR(&v1, p2, p1);
    v3SubtractToR(&v2, p3, p1);
    v3CrossProduct(normal, &v1, &v2);
    v3Normalize(normal);
    //if (normal->z < 0.0f) vMultiply(normal, -1.0f);
}

void CDrawSurface::UpdateRenderBuffers(vector3* Vertices)
{
    int ij;
    vector3 n1, n2, n3, n4, n5, n6;
    
    for (int i = 1; i < m_iNumberWidth - 1; i++)
        for (int j = 1; j < m_iNumberHeight - 1; j++)
        {
            ij = i * m_iNumberHeight + j;
            
//            m_avecNormal[ij].x = (Vertices[ij - m_iNumberHeight].z - Vertices[ij + m_iNumberHeight].z) * 0.5f;
//            m_avecNormal[ij].y = (Vertices[ij - 1].z - Vertices[ij + 1].z) * 0.5;
//            m_avecNormal[ij].z = 4.0f;
            
            GetNormal(&n1, &Vertices[ij], &Vertices[ij - 1], &Vertices[ij + m_iNumberHeight]);
            GetNormal(&n2, &Vertices[ij], &Vertices[ij + m_iNumberHeight], &Vertices[ij + m_iNumberHeight + 1]);
            GetNormal(&n3, &Vertices[ij], &Vertices[ij + m_iNumberHeight + 1], &Vertices[ij + 1]);
            GetNormal(&n4, &Vertices[ij], &Vertices[ij + 1], &Vertices[ij - m_iNumberHeight]);
            GetNormal(&n5, &Vertices[ij], &Vertices[ij - m_iNumberHeight], &Vertices[ij -m_iNumberHeight - 1]);
            GetNormal(&n6, &Vertices[ij], &Vertices[ij - m_iNumberHeight - 1], &Vertices[ij - 1]);
            
            v3Add(&n1, &n2);
            v3Add(&n1, &n3);
            v3Add(&n1, &n4);
            v3Add(&n1, &n5);
            v3Add(&n1, &n6);
            v3Normalize(&n1);

            v3Copy(&m_avecNormal[ij], &n1);
        }
    
    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector3) * m_iTotalNumber, static_cast<void*>(Vertices), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector3) * m_iTotalNumber, static_cast<void*>(m_avecNormal), GL_STATIC_DRAW);
}

void CDrawSurface::Render()
{
    glBindVertexArrayOES(m_gVAO);
    glDrawElements(GL_TRIANGLES, m_iTotalNumberIndex, GL_UNSIGNED_INT, (void*)0);
}

//void CDrawSurface::fileit()
//{
//    for (int i = 1; i < m_iNumberWidth - 1; i++)
//        for (int j = 1; j < m_iNumberHeight - 1; j++)
//        {
//            int ij = i * m_iNumberHeight + j;
//            
//            if (m_avecNormal[ij].x != 0.0f || m_avecNormal[ij].y != 0.0f)
//            __LOGARG("%i : %f %f %f", ij, m_avecNormal[ij].x, m_avecNormal[ij].y,
//                     m_avecNormal[ij].z);
//        }
//    
//}

// ---- CHeightField ----
//
//CHeightField::CHeightField()
//{
//    
//}
//
//CHeightField::~CHeightField()
//{
//    
//}
//
//bool CHeightField::Init(void *Property)
//{
////    assert(Property);
////    if (!Property) return Property;
//    
//    m_opMaterial = new CMaterialDefault(this);
//    m_opMaterial->Init();
//    
//    int id = 0, k = 0;
//    float originX = -16.0f;
//    float originY = -24.0f;
//    float x, y;
//    
//    for (int i = 0; i < HEIGHTFIELD_W; i++)
//    {
//        x = originX + ((float)i) * 1.0f;
//        
//        for (int j = 0; j < HEIGHTFIELD_H; j++)
//        {
//            id = i * HEIGHTFIELD_H + j;
//            y = originY + ((float)j) * 1.0f;
//            vSet(&m_avecVertex[id], x, y, 0.0f);
//            vSet(&m_avecNormal[id], 0.0f, 0.0f, 1.0f);
//            m_afHeight[id] = 1.0f;
//            m_afVelocity[id] = 0.0f;
//        }
//    }
//    
//    for (int i = 0; i < HEIGHTFIELD_W - 1; i++)
//        for (int j = 0; j < HEIGHTFIELD_H - 1; j++)
//        {
//            id = i * HEIGHTFIELD_H + j;
//            m_aiIndex[k++] = id;
//            m_aiIndex[k++] = id + HEIGHTFIELD_H;
//            m_aiIndex[k++] = id + HEIGHTFIELD_H + 1;
//            
//            m_aiIndex[k++] = id;
//            m_aiIndex[k++] = id + HEIGHTFIELD_H + 1;
//            m_aiIndex[k++] = id + 1;
//        }
//    
//    glGenVertexArraysOES(1, &m_gVAO);
//    glBindVertexArrayOES(m_gVAO);
//    
//    glGenBuffers(3, m_gVBO);
//    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO[0]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(m_avecVertex), static_cast<void*>(m_avecVertex), GL_STATIC_DRAW);
//    glEnableVertexAttribArray(SAL_POSITION);
//    glVertexAttribPointer(SAL_POSITION, 3, GL_FLOAT, GL_FALSE, 12, 0);
//    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO[1]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(m_avecNormal), static_cast<void*>(m_avecNormal), GL_STATIC_DRAW);
//    glEnableVertexAttribArray(SAL_NORMAL);
//    glVertexAttribPointer(SAL_NORMAL, 3, GL_FLOAT, GL_FALSE, 12, 0);
//    
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gVBO[2]);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * HEIGHTFIELD_I, static_cast<void*>(m_aiIndex), GL_STATIC_DRAW);
//    
//    glBindVertexArrayOES(0);
//    
//    return true;
//}
//
//void CHeightField::Deinit()
//{
//    if (m_opMaterial)
//    {
//        m_opMaterial->Deinit();
//        delete m_opMaterial;
//    }
//    
//    glDeleteBuffers(3, m_gVBO);
//    glDeleteVertexArraysOES(1, &m_gVAO);
//}
//
//E_MESSAGE_RESULT CHeightField::HandleMessage(const CMessage &Message)
//{
//    switch(Message.m_eType)
//    {
//        case MT_UPDATE:
//        {
//            Update();
//            
//            return MR_SUCCESS;
//        }
//            
//        case MT_RENDER:
//        {
//            SMaterialRenderData data = {m_opObject->GetTransformationMatrix(), nullptr};
//            
//            m_opMaterial->Render(&data);
//            
//            return MR_SUCCESS;
//        }
//            
//        case MT_TOUCHED_START:
//        {
//            Touch(static_cast<STouchData*>(Message.m_vpData));
//            
//            return MR_SUCCESS;
//        }
//    }
//    
//    return MR_IGNORE;
//}
//
//void CHeightField::Register()
//{
//    Globals::GetObjectManager().RegisterComponent(CI_HEIGHTFIELD, Create, Destroy);
//    Globals::GetObjectManager().SubscribeMessage(CI_HEIGHTFIELD, MT_UPDATE);
//    Globals::GetObjectManager().SubscribeMessage(CI_HEIGHTFIELD, MT_RENDER);
//    Globals::GetObjectManager().SubscribeMessage(CI_HEIGHTFIELD, MT_TOUCHED_START);
//}
//
//IComponent* CHeightField::Create()
//{
//    return new CHeightField;
//}
//
//void CHeightField::Destroy(IComponent *Component)
//{
//    assert(Component);
//    delete Component;
//}
//
//void CHeightField::Update()
//{
//    static float damping = 0.99f;
//    int ij = 0;
//    
//    for (int i = 1; i < HEIGHTFIELD_W - 1; i++)
//        for (int j = 1; j < HEIGHTFIELD_H - 1; j++)
//        {
//            ij = i * HEIGHTFIELD_H + j;
//            
//            m_afVelocity[ij] += (m_afHeight[ij - HEIGHTFIELD_H] +
//                                 m_afHeight[ij + HEIGHTFIELD_H] +
//                                 m_afHeight[ij - 1] +
//                                 m_afHeight[ij + 1]) * 0.25f - m_afHeight[ij];
//            m_afVelocity[ij] *= damping;
//        }
//    for (int i = 1; i < HEIGHTFIELD_W - 1; i++)
//        for (int j = 1; j < HEIGHTFIELD_H - 1; j++)
//        {
//            ij = i * HEIGHTFIELD_H + j;
//            
//            m_afHeight[ij] += m_afVelocity[ij];
//            m_avecVertex[ij].z = m_afHeight[ij];
//        }
//    for (int i = 1; i < HEIGHTFIELD_W - 1; i++)
//        for (int j = 1; j < HEIGHTFIELD_H - 1; j++)
//        {
//            ij = i * HEIGHTFIELD_H + j;
//            m_avecNormal[ij].x = m_afHeight[ij - HEIGHTFIELD_H] - m_afHeight[ij + HEIGHTFIELD_H];
//            m_avecNormal[ij].y = m_afHeight[ij - 1] - m_afHeight[ij + 1];
//            m_avecNormal[ij].z = 4.0f;
//            vNormalize(&m_avecNormal[ij]);
//        }
//    
//    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO[0]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(m_avecVertex), static_cast<void*>(m_avecVertex), GL_STATIC_DRAW);
//    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO[1]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(m_avecNormal), static_cast<void*>(m_avecNormal), GL_STATIC_DRAW);
//}
//
//void CHeightField::Render()
//{
//    glBindVertexArrayOES(m_gVAO);
//    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gVBO[2]);
//    //glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, 0);
//    //    glDrawArrays(GL_POINTS, 0, HEIGHTFIELD_N);
//    //glDrawArrays(GL_TRIANGLES, 0, 1000);//HEIGHTFIELD_I);
//    glDrawElements(GL_TRIANGLES, HEIGHTFIELD_I, GL_UNSIGNED_INT, (void*)0);
//}
//
//void CHeightField::Touch(STouchData *Data)
//{    
//    int x = (int)((float_t)(HEIGHTFIELD_W - 1) * (0.5f + Data->x *  (1.0f / Globals::GetScreenWidth())));
//    int y = (int)((float_t)(HEIGHTFIELD_H - 1) * (0.5f + Data->y * (1.0f / Globals::GetScreenHeight())));
//    
//    m_afHeight[x * HEIGHTFIELD_H + y] -= 2.0f;
//}

// CHeightFieldQuad
//
//CHeightFieldQuad::CHeightFieldQuad()
//{
//    m_iNumWeightWidth = 0;
//    m_iNumWeightHeight = 0;
//    m_iNumTotalWeight = 0;
//    m_iNumVertex = 0;
//    
//    m_afWeight = NULL;
//    m_afVelocity = NULL;
//    m_avecVertex = NULL;
//}
//
//CHeightFieldQuad::~CHeightFieldQuad()
//{
//    if (m_afWeight) delete m_afWeight;
//    if (m_afVelocity) delete m_afVelocity;
//    if (m_avecVertex) delete m_avecVertex;
//}
//
//bool CHeightFieldQuad::Init()
//{
//    int id = 0;
//    float originX = -16.0f;
//    float originY = -24.0f;
//    float x, y;
//    
//    // init params
//    m_iNumWeightWidth = 34;
//    m_iNumWeightHeight = 50;
//    m_iNumTotalWeight = 34 * 50;
//    m_iNumVertex = 32 * 48 * 4;
//    
//    // init material
//    m_opMaterial = new CMaterialHF(this);
//    m_opMaterial->Init();
//    
//    // allocate memory
//    m_afWeight = new float[m_iNumTotalWeight];
//    m_afVelocity = new float[m_iNumTotalWeight];
//    
//    m_avecVertex = new vector3[m_iNumVertex];
////    m_aiIndex = new int[m_iNumIndex];
//    
//    // fill memory
//    for (int i = 0; i < m_iNumWeightWidth; i++)
//        for (int j = 0; j < m_iNumWeightHeight; j++)
//        {
//            id = i * m_iNumWeightHeight + j;
//            m_afWeight[id] = 0.0f;
//            m_afVelocity[id] = 0.0f;
//        }
//    
//    id = 0;
//    for (int i = 0; i < m_iNumWeightWidth - 2; i++)
//    {
//        x = originX + ((float)i) * 1.0f;
//        
//        for (int j = 0; j < m_iNumWeightHeight - 2; j++)
//        {
//            y = originY + ((float)j) * 1.0f;
//
//            vSet(&m_avecVertex[id++], x, y, 0.0f);
//            vSet(&m_avecVertex[id++], x + 1.0f, y, 0.0f);
//            vSet(&m_avecVertex[id++], x, y + 1.0f, 0.0f);
//            vSet(&m_avecVertex[id++], x + 1.0f, y + 1.0f, 0.0f);
//        }
//    }
//    
////    for (int i = 0; i < m_iNumWeightWidth - 2; i++)
////        for (int j = 0; j < m_iNumWeightHeight - 2; j++)
////        {
////            id = i * tempNum + j;
////            m_aiIndex[k++] = id;
////            m_aiIndex[k++] = id + tempNum;
////            m_aiIndex[k++] = id + tempNum + 1;
////            
////            m_aiIndex[k++] = id;
////            m_aiIndex[k++] = id + tempNum + 1;
////            m_aiIndex[k++] = id + 1;
////        }
//    
//    glGenVertexArraysOES(1, &m_gVAO);
//    glBindVertexArrayOES(m_gVAO);
//    
//    glGenBuffers(1, &m_gVBO);
//    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vector3) * m_iNumVertex, static_cast<void*>(m_avecVertex), GL_STATIC_DRAW);
//    glEnableVertexAttribArray(SAL_POSITION);
//    glVertexAttribPointer(SAL_POSITION, 3, GL_FLOAT, GL_FALSE, 12, 0);
//    
////    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gVBO[1]);
////    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_iNumIndex, static_cast<void*>(m_aiIndex), GL_STATIC_DRAW);
//    
//    glBindVertexArrayOES(0);
//    
//    return true;
//}
//
//void CHeightFieldQuad::Deinit()
//{
//    if (m_opMaterial)
//    {
//        m_opMaterial->Deinit();
//        delete m_opMaterial;
//    }
//    
//    glDeleteBuffers(1, &m_gVBO);
//    glDeleteVertexArraysOES(1, &m_gVAO);
//}
//
//E_MESSAGE_RESULT CHeightFieldQuad::HandleMessage(const CMessage &Message)
//{
//    switch(Message.m_eType)
//    {
//        case MT_UPDATE:
//        {
//            Update();
//            
//            return MR_SUCCESS;
//        }
//        
//        case MT_RENDER:
//        {
//            m_opMaterial->Render(m_opObject->GetTransformationMatrix());
//            
//            return MR_SUCCESS;
//        }
//        
//        case MT_TOUCHED_START:
//        {
//            Touch(static_cast<STouchData*>(Message.m_vpData));
//            
//            return MR_SUCCESS;
//        }
//    }
//    
//    return MR_IGNORE;
//}
//
//void CHeightFieldQuad::Register()
//{
//    Globals::GetObjectManager().RegisterComponent(CI_HEIGHTFIELDQUAD, Create, Destroy);
//    Globals::GetObjectManager().SubscribeMessage(CI_HEIGHTFIELDQUAD, MT_UPDATE);
//    Globals::GetObjectManager().SubscribeMessage(CI_HEIGHTFIELDQUAD, MT_RENDER);
//    Globals::GetObjectManager().SubscribeMessage(CI_HEIGHTFIELDQUAD, MT_TOUCHED_START);
//}
//
//IComponent* CHeightFieldQuad::Create()
//{
//    return new CHeightFieldQuad;
//}
//
//void CHeightFieldQuad::Destroy(IComponent *Component)
//{
//    assert(Component);
//    delete Component;
//}
//
//void CHeightFieldQuad::Update()
//{
//    static float damping = 0.99f;
//    int ij = 0, id = 0;
//    
//    for (int i = 1; i < m_iNumWeightWidth - 1; i++)
//        for (int j = 1; j < m_iNumWeightHeight - 1; j++)
//        {
//            ij = i * m_iNumWeightHeight + j;
//            
//            m_afVelocity[ij] += (m_afWeight[ij - m_iNumWeightHeight] +
//                                 m_afWeight[ij + m_iNumWeightHeight] +
//                                 m_afWeight[ij - 1] +
//                                 m_afWeight[ij + 1]) * 0.25f - m_afWeight[ij];
//            m_afVelocity[ij] *= damping;
//        }
//    
//    for (int i = 1; i < m_iNumWeightWidth - 1; i++)
//        for (int j = 1; j < m_iNumWeightHeight - 1; j++)
//        {
//            ij = i * m_iNumWeightHeight + j;
//            
//            m_afWeight[ij] += m_afVelocity[ij];
//            
//            m_avecVertex[id++].z = m_afWeight[ij];
//            m_avecVertex[id++].z = m_afWeight[ij];
//            m_avecVertex[id++].z = m_afWeight[ij];
//            m_avecVertex[id++].z = m_afWeight[ij];
//        }
//    
//    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vector3) * m_iNumVertex, static_cast<void*>(m_avecVertex), GL_STATIC_DRAW);
//}
//
//void CHeightFieldQuad::Render()
//{
//    glBindVertexArrayOES(m_gVAO);
//    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gVBO[2]);
//    //glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, 0);
//    //    glDrawArrays(GL_POINTS, 0, HEIGHTFIELD_N);
//    //glDrawArrays(GL_TRIANGLES, 0, 1000);//HEIGHTFIELD_I);
//    //glDrawElements(GL_TRIANGLE_STRIP, m_iNumVertex, GL_UNSIGNED_INT, (void*)0);
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, m_iNumVertex);
//}
//
//void CHeightFieldQuad::Touch(STouchData *Data)
//{
//    
//    int x = (int)((float_t)(m_iNumWeightWidth - 2) * (0.5f + Data->x *  (1.0f / Globals::GetScreenWidth())));
//    int y = (int)((float_t)(m_iNumWeightHeight - 2) * (0.5f + Data->y * (1.0f / Globals::GetScreenHeight())));
//    
//    m_afWeight[x * m_iNumWeightHeight + y] -= 10.0f;
//}

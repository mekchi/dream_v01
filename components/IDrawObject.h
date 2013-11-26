//
//  IDrawObject.h
//  dream
//
//  Created by Yermek Garifullanov on 10/7/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef __dream__IDrawObject__
#define __dream__IDrawObject__

#include "Graphics.h"
#include "IComponent.h"
#include "Ids.h"
#include "DataTypes.h"
#include "VQMMath.h"

class IMaterial;

class IDrawObject : public IComponent
{
    
public:
    
    IDrawObject() : m_opMaterial(NULL) {}
    virtual ~IDrawObject() {}
    
    virtual E_FAMILY_COMPONENT_ID GetFamilyComponentId () { return FCI_DRAW_OBJECT; }
    
    virtual void Render() = 0;
    
protected:
    
    IMaterial *m_opMaterial;
    
};

class CDrawSphere : public IDrawObject
{
    
public:
    
    CDrawSphere();
    virtual ~CDrawSphere();
    
    virtual bool Init(void* Property);
    virtual void Deinit();

    virtual E_COMPONENT_ID GetComponentId() { return CI_DRAW_SPHERE; }
    
    virtual E_MESSAGE_RESULT HandleMessage(const CMessage& Message);

    static void Register();
    static IComponent* Create();
    static void Destroy(IComponent* Component);
    
    virtual void Render();

private:
    
    GLuint m_gVAO;
    GLuint m_gVBO;
    GLuint m_gIndices;
    
    int m_iNumFaceVertex;
    
};

class CDrawSurface : public IDrawObject
{
public:
    
    CDrawSurface();
    virtual ~CDrawSurface();
    
    virtual bool Init(void* Property);
    virtual void Deinit();
    
    virtual E_MESSAGE_RESULT HandleMessage(const CMessage& Message);
    
    virtual E_COMPONENT_ID GetComponentId() { return CI_DRAW_SURFACE; }
    
    static void Register();
    static IComponent* Create();
    static void Destroy(IComponent* Component);
    
    void Render();
    
private:
    
    void GetNormal(vector3* normal, vector3* p1, vector3* p2, vector3 *p3);
    
    void UpdateRenderBuffers(vector3* Vertices);
    void Touch(STouchData* Data);
    
//    void fileit();
    
    int m_iNumberWidth; // number of points along x asix
    int m_iNumberHeight; // number of points along y axis
    int m_iTotalNumber;

    // graphics property
    
    int m_iTotalNumberIndex; // number of indices
    int* m_aiIndex; // array of indices
    vector3* m_avecNormal;
    vector3* m_vecProtagonist; // protagonist position
    float m_fWaveRadius;
    
    GLuint m_gVAO;
    GLuint m_gVBO[3];
    
//    IMaterial* waveTest;
};
//
//class CHeightField : public IDrawObject
//{
//    
//public:
//    
//    CHeightField();
//    virtual ~CHeightField();
//    
//    virtual bool Init(void* Property);
//    virtual void Deinit();
//    
//    virtual E_MESSAGE_RESULT HandleMessage(const CMessage& Message);
//    
//    virtual E_COMPONENT_ID GetComponentId() { return CI_HEIGHTFIELD; }
//    
//    static void Register();
//    static IComponent* Create();
//    static void Destroy(IComponent* Component);
//    
//    void Render();
//    
//private:
//    
//    void Update();
//    void Touch(STouchData* Data);
//    
//#define HEIGHTFIELD_N 1617
//#define HEIGHTFIELD_W 33
//#define HEIGHTFIELD_H 49
//#define HEIGHTFIELD_I 9216
//    
//    float m_afHeight[HEIGHTFIELD_N];
//    float m_afVelocity[HEIGHTFIELD_N];
//    float m_fDamping;
//    
//    // render attributes
//    vector3 m_avecVertex[HEIGHTFIELD_N];
//    vector3 m_avecNormal[HEIGHTFIELD_N];
//    int m_aiIndex[HEIGHTFIELD_I];
//    
//    GLuint m_gVAO;
//    GLuint m_gVBO[3];
//    //    GLuint m_gIndices;
//};


//
//
//class CHeightFieldQuad : public IDrawObject
//{
//    
//public:
//    
//    CHeightFieldQuad();
//    virtual ~CHeightFieldQuad();
//    
//    virtual bool Init();
//    virtual void Deinit();
//    
//    virtual E_MESSAGE_RESULT HandleMessage(const CMessage& Message);
//    
//    virtual E_COMPONENT_ID GetComponentId() { return CI_HEIGHTFIELD; }
//    
//    static void Register();
//    static IComponent* Create();
//    static void Destroy(IComponent* Component);
//    
//    void Render();
//    
//private:
//    
//    void Update();
//    void Touch(STouchData* Data);
//    
//    // physics
//    int m_iNumWeightWidth;
//    int m_iNumWeightHeight;
//    int m_iNumTotalWeight;
//    
//    float* m_afWeight;
//    float* m_afVelocity;
//    
//    float m_fDamping;
//    
//    // render attributes
//    int m_iNumVertex;
//
//    vector3 *m_avecVertex;
////    int *m_aiIndex;
//    
//    GLuint m_gVAO;
//    GLuint m_gVBO;
//};

#endif /* defined(__dream__IDrawObject__) */

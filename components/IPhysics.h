//
//  IPhysics.h
//  dream
//
//  Created by Yermek Garifullanov on 11/5/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef __dream__IPhysics__
#define __dream__IPhysics__

#include "IComponent.h"

#include "DataTypes.h"
#include "VQMMath.h"

class IPhysics : public IComponent
{
    
public:
    
    IPhysics() {}
    virtual ~IPhysics() {}
    
    virtual E_FAMILY_COMPONENT_ID GetFamilyComponentId() { return FCI_PHYSICS; }
    
};

// ---- CPhysicsProtagonist ----

class CPhysicsProtagonist : public IPhysics
{
    
public:
    
    CPhysicsProtagonist();
    virtual ~CPhysicsProtagonist();
    
    virtual bool Init(void* Property);
    virtual void Deinit();
    
    virtual E_MESSAGE_RESULT HandleMessage(const CMessage& Message);
    
    virtual E_COMPONENT_ID GetComponentId() { return CI_PHYSICS_PROTAGONIST; }
    
    static void Register();
    static IComponent* Create();
    static void Destroy(IComponent* Component);

private:
    
    void Update(float DeltaTime);
    
};

// ---- CPhysicsEnvironment ----

class CPhysicsEnvironment : public IPhysics
{
public:
    
    CPhysicsEnvironment();
    virtual ~CPhysicsEnvironment();
    
    virtual bool Init(void* Property);
    virtual void Deinit();
    
    virtual E_MESSAGE_RESULT HandleMessage(const CMessage& Message);
    
    virtual E_COMPONENT_ID GetComponentId() { return CI_PHYSICS_HEIGHTFIELD; }
    
    static void Register();
    static IComponent* Create();
    static void Destroy(IComponent* Component);
    
private:
    
    void ComputeForce(SObjectData *Data);
    void Update(float DeltaTime);
    void Touch(STouchData* Data);
    
    int m_iNumberWidth; // number of heights along x axis
    int m_iNumberHeight; // number of heights along y axis
    int m_iTotalNumber;
    
    // origin point of the field
    float m_fOriginX;
    float m_fOriginY;
    float m_fLevel; // z coordinate
    
    float m_fStep; // square grid step
    
    vector3* m_avecPosition;
    float* m_afVelocity;
    
};

// ---- CPhysicsForce ----

typedef struct
{
    
    vector3 Position;
    vector3 Velocity;
    
} State;

typedef struct
{
    
    vector3 dPosition;
    vector3 dVelocity;
    
} Derivative;

class CPhysicsForce : public IPhysics
{
    
public:
    
    CPhysicsForce();
    virtual ~CPhysicsForce();
    
    virtual bool Init(void* Property);
    virtual void Deinit();
    
    virtual E_MESSAGE_RESULT HandleMessage(const CMessage& Message);
    
    virtual E_COMPONENT_ID GetComponentId() { return CI_PHYSICS_FORCE; }
    
    static void Register();
    static IComponent* Create();
    static void Destroy(IComponent* Component);

private:
    
    void Update(float DeltaTime);
    void Touch(vector3* Point);
    
    void LinearIntegration(float DeltaTime);
    void AngularIntegration(float DeltaTime);
    
//    void TouchSurface(vector3* Point);
//    void TouchObject(vector3* Point);
    
//    void Evalute(Derivative* Result, State* Initial, Derivative* Derivative, float DeltaTime);
//    void RK4(float DeltaTime);

    
    
    bool m_bTouched;
    float m_fObjectRadius; // kinda bounding box
    float m_fMass;

    vector3 m_vecPull;

    // linear
    vector3 m_vecForce;
    vector3 m_vecLinearVelocity;
//    vector3 m_vecLinearAcceleration;
    float m_fLinearDamping;

    // angular
    vector3 m_vecTorque;
    vector3 m_vecAngularVelocity;
    float m_fAngularDamping;
//    quaternion m_quat
    
    // wave
    float m_fWaveRadius; // wave radius if object touched
    float m_fWaveRadiusStep;
    float m_fWaveLifeTime;
    float m_fWaveMaxLifeTime;
};

// ---- CPhysicsHeightField ----

class CPhysicsHeightField : public IPhysics
{
    
public:
    
    CPhysicsHeightField();
    virtual ~CPhysicsHeightField();
    
    virtual bool Init(void* Property);
    virtual void Deinit();
    
    virtual E_MESSAGE_RESULT HandleMessage(const CMessage& Message);
    
    virtual E_COMPONENT_ID GetComponentId() { return CI_PHYSICS_HEIGHTFIELD; }
    
    static void Register();
    static IComponent* Create();
    static void Destroy(IComponent* Component);
    
private:
    
    void ComputeForce(SObjectData *Data);
    void Update(float DeltaTime);
    void Touch(STouchData* Data);
    
    int m_iNumberWidth; // number of heights along x axis
    int m_iNumberHeight; // number of heights along y axis
    int m_iTotalNumber;
    
    // origin point of the field
    float m_fOriginX;
    float m_fOriginY;
    float m_fLevel; // z coordinate
    
    float m_fStep; // square grid step
    
    vector3* m_avecPosition;
    float* m_afVelocity;
};


#endif /* defined(__dream__IPhysics__) */

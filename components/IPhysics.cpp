//
//  IPhysics.cpp
//  dream
//
//  Created by Yermek Garifullanov on 11/5/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#include "IPhysics.h"

#include <assert.h>

#include "FastMath.h"
#include "Globals.h"
#include "CObjectManager.h"
#include "CMessage.h"

static vector3 gravity = {0.0f, 0.0f, -9.8f};

// ---- class CPhysicsForce ----

CPhysicsForce::CPhysicsForce()
{
    
}

CPhysicsForce::~CPhysicsForce()
{
    
}

bool CPhysicsForce::Init(void* Property)
{
//    vSetZero(&m_vecLinearAcceleration);
    vSetZero(&m_vecForce);
    vSetZero(&m_vecTorque);
    vSetZero(&m_vecLinearVelocity);
    vSetZero(&m_vecAngularVelocity);
    
    m_fLinearDamping = 0.95f;
    m_fAngularDamping = 0.9f;
    
    vSetZero(&m_vecPull);
    m_fMass = 500.0f;
    m_fObjectRadius = 1.0f;

    


    m_fWaveMaxLifeTime = 3.0f;
    m_fWaveRadiusStep = 16.0f / m_fWaveMaxLifeTime;    // step = total radius / total time

    m_fWaveRadius = 0.0f;
    m_fWaveLifeTime = 0.0;
    m_bTouched = false;
    
    return true;
}

void CPhysicsForce::Deinit()
{
    
}

E_MESSAGE_RESULT CPhysicsForce::HandleMessage(const CMessage &Message)
{
    switch (Message.m_eType)
    {
        case MT_UPDATE:
        {
            Update(*(static_cast<float*>(Message.m_vpData)));
            
            return MR_SUCCESS;
        }
            
        case MT_TOUCHED_START:
        {
            Touch(static_cast<vector3*>(Message.m_vpData));
         
            return MR_SUCCESS;
        }
    }
    
    return MR_IGNORE;
}

void CPhysicsForce::Register()
{
    Globals::GetObjectManager().RegisterComponent(CI_PHYSICS_FORCE, Create, Destroy);
    Globals::GetObjectManager().SubscribeMessage(CI_PHYSICS_FORCE, MT_UPDATE);
    Globals::GetObjectManager().SubscribeMessage(CI_PHYSICS_FORCE, MT_TOUCHED_START);
}

IComponent* CPhysicsForce::Create()
{
    return new CPhysicsForce();
}

void CPhysicsForce::Destroy(IComponent *Component)
{
    assert(Component);
    delete Component;
}

void CPhysicsForce::Update(float DeltaTime)
{
    if (m_bTouched)
    {
        m_fWaveLifeTime -= DeltaTime;

        if (m_fWaveLifeTime <= 0.0f)
        {
            m_fWaveRadius -= DeltaTime * m_fWaveRadiusStep;
        }
        else
        {
            m_fWaveRadius += DeltaTime * m_fWaveRadiusStep;
        }
        
        if (m_fWaveRadius < 0.0f)
        {
            m_bTouched = false;
            m_fWaveRadius = 0.0f;
        }
        else
        {
            Globals::GetObjectManager().BroadcastMessage(CMessage(MT_PROTAGONIST_TOUCH, static_cast<void*>(&m_fWaveRadius)));
        }
        
    }
    else
    {
//        vector3 force;
        

        // compute forces
        SObjectData data = {*(m_opObject->GetPosition()), m_fObjectRadius, &m_vecForce, &m_vecTorque};
        
        Globals::GetObjectManager().BroadcastMessage(CMessage(MT_PROTAGONIST_COMPUTE_FORCE,
                                                              static_cast<void*>(&data)));
      
        vAdd(&m_vecForce, &m_vecPull);
//        vDivideToR(&m_vecLinearAcceleration, &force, m_fMass);
//        vAdd(&m_vecLinearAcceleration, &m_vecPull);
//        vAdd(&m_vecLinearAcceleration, &gravity);
        
        LinearIntegration(DeltaTime);

        AngularIntegration(DeltaTime);
        
        
        // reset acceleration
//        vSetZero(&m_vecLinearAcceleration);
        vSetZero(&m_vecForce);
        vSetZero(&m_vecTorque);
        vSetZero(&m_vecPull);
    }
}

void CPhysicsForce::Touch(vector3 *Point)
{
    vector3 coord;
    vector3 v;
    
    vSet(&coord, Point->x - Globals::GetScreenWidth() * 0.5f,
         Point->y - Globals::GetScreenHeight() * 0.5f,
         m_opObject->GetPosition()->z);
    
    vSubtracToR(&v, &coord, m_opObject->GetPosition());
    if (vMagnitude(&v) > m_fObjectRadius)
    {
        // surface touch
        vCopy(&m_vecPull, &coord);
        vSubtract(&m_vecPull, m_opObject->GetPosition());
        vNormalize(&m_vecPull);
        vMultiply(&m_vecPull, 100000.0f);
    }
    else
    {
        m_opObject->ResetRotaiton();
        m_fWaveLifeTime = m_fWaveMaxLifeTime;
        m_bTouched = true;
    }
}

// direct linear integration
void CPhysicsForce::LinearIntegration(float DeltaTime)
{
    vector3 dp, temp, acceleration;
    
    // damping
//    vMultiplyToR(&temp, &m_vecLinearVelocity, m_fLinearDamping);
//    vSubtract(&m_vecForce, &temp);
    
    vDivideToR(&acceleration, &m_vecForce, m_fMass);
    vAdd(&acceleration, &gravity);
    //vAdd(&m_vecLinearAcceleration, &m_vecPull);
    
//    vMultiplyToR(&temp, &m_vecLinearVelocity, DeltaTime);
//    vAdd(&temp, m_opObject->GetPosition());
//    m_opObject->SetPosition(&temp);
//    
//    vMultiplyToR(&temp, &acceleration, DeltaTime);
//    vAdd(&m_vecLinearVelocity, &temp);
//    vMultiply(&m_vecLinearVelocity, m_fLinearDamping);
    
    vCopy(&dp, &m_vecLinearVelocity);
    vMultiply(&dp, DeltaTime);
    vMultiplyToR(&temp, &acceleration, DeltaTime * DeltaTime * 0.5f);
    vAdd(&dp, &temp);
    
    // new position
    vAdd(&dp, m_opObject->GetPosition());
    m_opObject->SetPosition(&dp);
    
    // new velocity
    vMultiplyToR(&temp, &acceleration, DeltaTime);
    vAdd(&m_vecLinearVelocity, &temp);
    vMultiply(&m_vecLinearVelocity, m_fLinearDamping);
}

// direct angular integration
void CPhysicsForce::AngularIntegration(float DeltaTime)
{
    const float inertia = 0.4f * m_fMass * m_fObjectRadius * m_fObjectRadius;
    const float invert_inertia = 1.0f / inertia;
    
//    vector3 v, cross, acceleration;
//    quaternion qNew, *qOld = m_opObject->GetRotation();
    
    vector3 dv, da, acceleration;
    

    vMultiplyToR(&acceleration, &m_vecTorque, invert_inertia);
    
    vCopy(&dv, &m_vecAngularVelocity);
    vMultiply(&dv, DeltaTime);
    vMultiplyToR(&da, &acceleration, DeltaTime * DeltaTime * 0.5f);
    vAdd(&dv, &da);
    
    // new rotaion
    vAdd(&dv, m_opObject->GetRotation());
    m_opObject->SetRotation(&dv);
    
    // new velocity
    vMultiplyToR(&da, &acceleration, DeltaTime);
    vAdd(&m_vecAngularVelocity, &da);
    vMultiply(&m_vecAngularVelocity, m_fAngularDamping);
    
    
    // damping
//    vMultiplyToR(&v, &m_vecAngularVelocity, m_fAngularDamping);
//    vAdd(&m_vecTorque, &v);
    
//    vAngularVelocityToQuaternion(&qNew, &m_vecAngularVelocity);
//    qMultiply(&qNew, qOld);
//    qScale(&qNew, 0.5f * DeltaTime);
//    qAdd(&qNew, qOld);
//    qNormalize(&qNew);
//    
//    m_opObject->SetRotation(&qNew);
//    
//    vMultiplyToR(&v, &m_vecAngularVelocity, inertia);
//    vCrossProduct(&cross, &m_vecAngularVelocity, &v);
//    vSubtracToR(&v, &m_vecTorque, &cross);
//    vMultiply(&v, DeltaTime * invert_inertia);
//    vAdd(&m_vecAngularVelocity, &v);
//    vMultiply(&m_vecAngularVelocity, m_fAngularDamping);
}

//void CPhysicsForce::Evalute(Derivative* Result, State* Initial, Derivative* Derivative, float DeltaTime)
//{
//    vector3 statePosition;
//    
//    vMultiplyToR(&statePosition, &(Derivative->dPosition), DeltaTime);
//    vAdd(&statePosition, &(Initial->Position));
//    
//    // derivative of position
//    vMultiplyToR(&(Result->dPosition), &(Derivative->dVelocity), DeltaTime);
//    vAdd(&(Result->dPosition), &(Initial->Velocity));
//    
//    // derivative of velocity
//    vCopy(&(Result->dVelocity), &m_vecLinearAcceleration);
//    //Acceleration(&(Result->dVelocity), &statePosition, &(Result->dPosition));
//}
//
//void CPhysicsForce::RK4(float DeltaTime)
//{
//    const float velocity_damping = 0.9f;
//    
//    vector3 zero = {0.0f, 0.0f, 0.0f};
//    vector3 dpdt, dvdt;
//    State state = {*(m_opObject->GetPosition()), m_vecLinearVelocity};
//    Derivative d0 = {zero, zero};
//    Derivative d1, d2, d3, d4;
//    
//    Evalute(&d1, &state, &d0, 0.0f);
//    Evalute(&d2, &state, &d1, DeltaTime * 0.5f);
//    Evalute(&d3, &state, &d2, DeltaTime * 0.5f);
//    Evalute(&d4, &state, &d3, DeltaTime);
//    
//    // new position
//    vAddToR(&dpdt, &(d2.dPosition), &(d3.dPosition));
//    vMultiply(&dpdt, 2.0f);
//    vAdd(&dpdt, &(d1.dPosition));
//    vAdd(&dpdt, &(d4.dPosition));
//    vMultiply(&dpdt, 1.0f / 6.0f);
//    
//    vMultiply(&dpdt, DeltaTime);
//    vAdd(&dpdt, m_opObject->GetPosition());
//    
//    m_opObject->SetPosition(&dpdt);
//    
//    // new velocity
//    vAddToR(&dvdt, &(d2.dVelocity), &(d3.dVelocity));
//    vMultiply(&dvdt, 2.0f);
//    vAdd(&dvdt, &(d1.dVelocity));
//    vAdd(&dvdt, &(d4.dVelocity));
//    vMultiply(&dvdt, 1.0f / 6.0f);
//    
//    vMultiply(&dvdt, DeltaTime);
//    vMultiply(&m_vecLinearVelocity, velocity_damping);
//    vAdd(&m_vecLinearVelocity, &dvdt);
//}



// ---- class CPhysicsForce ----

CPhysicsHeightField::CPhysicsHeightField()
{
    m_avecPosition = NULL;
    m_afVelocity = NULL;
}

CPhysicsHeightField::~CPhysicsHeightField()
{
    
}

bool CPhysicsHeightField::Init(void* Property)
{
    assert(Property);
    if (!Property) return false;
    
    SHeightFieldProperty *property = static_cast<SHeightFieldProperty*>(Property);

    m_fStep = property->GridStep;
    m_iNumberWidth = property->NumberWidthHeight + 2;
    m_iNumberHeight = property->NumberHeightHeight + 2;
    m_iTotalNumber = m_iNumberWidth * m_iNumberHeight;
    
    m_avecPosition = new vector3[m_iTotalNumber];
    m_afVelocity = new float[m_iTotalNumber];

    float x, y;
    int id;
    
    m_fOriginX = property->OriginX - m_fStep;
    m_fOriginY = property->OriginY - m_fStep;
    m_fLevel = property->Level;
    
    for (int i = 0; i < m_iNumberWidth; i++)
    {
        x = m_fOriginX + ((float)i) * m_fStep;
        
        for (int j = 0; j < m_iNumberHeight; j++)
        {
            id = i * m_iNumberHeight + j;
            y = m_fOriginY + ((float)j) * m_fStep;
            
            vSet(&m_avecPosition[id], x, y, m_fLevel);
            m_afVelocity[id] = 0.0f;
        }
    }
    
    //m_avecPosition[975].z = 10.0f;
    
    return true;
}

void CPhysicsHeightField::Deinit()
{
    if (m_avecPosition) delete m_avecPosition;
    if (m_afVelocity) delete m_afVelocity;
}

E_MESSAGE_RESULT CPhysicsHeightField::HandleMessage(const CMessage &Message)
{
    switch (Message.m_eType)
    {
        case MT_UPDATE:
        {
            Update(*(static_cast<float*>(Message.m_vpData)));
            
//            Globals::GetObjectManager().BroadcastMessage(CMessage(MT_UPDATE_RENDER, static_cast<void*>(m_avecPosition)));

            
            return MR_SUCCESS;
        }
            
        case MT_PROTAGONIST_COMPUTE_FORCE:
        {
            ComputeForce(static_cast<SObjectData*>(Message.m_vpData));
            
            return MR_SUCCESS;
        }
            
        case MT_TOUCHED_START:
        {
            Touch(static_cast<STouchData*>(Message.m_vpData));
            
            return MR_SUCCESS;
        }
    }
    
    return MR_IGNORE;
}

void CPhysicsHeightField::Register()
{
    Globals::GetObjectManager().RegisterComponent(CI_PHYSICS_HEIGHTFIELD, Create, Destroy);
    Globals::GetObjectManager().SubscribeMessage(CI_PHYSICS_HEIGHTFIELD, MT_UPDATE);
    Globals::GetObjectManager().SubscribeMessage(CI_PHYSICS_HEIGHTFIELD,MT_PROTAGONIST_COMPUTE_FORCE);
    Globals::GetObjectManager().SubscribeMessage(CI_PHYSICS_HEIGHTFIELD, MT_TOUCHED_START);
}

IComponent* CPhysicsHeightField::Create()
{
    return new CPhysicsHeightField();
}

void CPhysicsHeightField::Destroy(IComponent *Component)
{
    assert(Component);
    delete Component;
}

void CPhysicsHeightField::ComputeForce(SObjectData *Data)
{
    const float ratio = -1000.0f * m_fStep * m_fStep;
    float radius = Data->Radius;
    vector3 center = Data->position;
    float diff = 0.0f, totalDiff = 0.0f;
    vector3 force;

    // level = 0 along z axis
    diff = center.z - radius;
    
    // check if lowest point of sphere below the level
    if (diff < m_fLevel)
    {
        vector3 torque, v, r;
        // check all points in square radius * 2 x radius * 2
        int idx = (int)((center.x - radius - m_fOriginX) / m_fStep);
        int idy = (int)((center.y - radius - m_fOriginY) / m_fStep);
        int n = 2.0f * radius / m_fStep;
        
        vSetZero(&torque);
        for (int x = 0; x < n; x++)
            for (int y = 0; y < n; y++)
            {
                int id = (idx + x) * m_iNumberHeight + (idy + y);
                float ax = center.x - m_avecPosition[id].x;
                float ay = center.y - m_avecPosition[id].y;
                float alength2 = ax * ax + ay * ay;
                
                if (alength2 <= radius * radius)
                {
                    float b = fastSqrt(radius * radius - alength2);
                    
                    vSet(&r, ax, ay, b);
                    vNormalize(&r);
                    vMultiply(&r, radius);
                    
                    ax = vMagnitude(&r); // test
                    if (center.z - b < m_fLevel)
                    {
                        diff = b - center.z + m_fLevel;
                        totalDiff += diff;
                        m_avecPosition[id].z -= diff;
                        
                        // calculate torque vector
                        vMultiplyToR(&force, &gravity, ratio * diff);
                        vCrossProduct(&v, &r, &force);
                        vAdd(&torque, &v);
                    }
                }
            }
        
        // compute and add force
        vMultiplyToR(&force, &gravity, -1000.0f * totalDiff * m_fStep * m_fStep);
        vAdd(Data->Force, &force);
        // add torque
        vAdd(Data->Torque, &torque);
    }
    
}


//void CPhysicsHeightField::ComputeForce(SObjectData *Data)
//{
//    float radius = Data->Radius;
//    vector3 position = Data->position;
//    vector3 force;
//    int idx, idy;
//    float t;
//    float utotal = 0.0f;
//    float u = radius - position.z;
//    
//    if (u > 0)
//    {
//        idx = (int)((position.x + 17.0f) / m_fStep);
//        idy = (int)((position.y + 25.0f) / m_fStep);
//        
//        t = position.x - m_avecPosition[idx * m_iNumberHeight + idy].x;
//        t = fastSqrt(radius * radius -  t * t);
//        u = t - position.z;
//        utotal += u;
//        m_avecPosition[idx  * m_iNumberHeight + idy].z -= u;
//
//        
//        int iradius = (radius / m_fStep);
//
//        for (int i = 1; i < iradius; i++)
//        {
//            
//            t = position.x - m_avecPosition[(idx - i) * m_iNumberHeight + idy].x;
//            t = fastSqrt(radius * radius -  t * t);
//            
//            if (t > position.z)
//            {
//                u = t - position.z;
//                utotal += u;
//                m_avecPosition[(idx - i) * m_iNumberHeight + idy].z -= u;
//            }
//            
//            t = position.x - m_avecPosition[(idx + i) * m_iNumberHeight + idy].x;
//            t = fastSqrt(radius * radius -  t * t);
//            
//            if (t > position.z)
//            {
//                u = t - position.z;
//                utotal += u;
//                m_avecPosition[(idx - i) * m_iNumberHeight + idy].z -= u;
//            }
//            
//            t = position.y - m_avecPosition[idx * m_iNumberHeight + idy - i].y;
//            t = fastSqrt(radius * radius -  t * t);
//            
//            if (t > position.z)
//            {
//                u = t - position.z;
//                utotal += u;
//                m_avecPosition[idx * m_iNumberHeight + idy - i].z -= u;
//            }
//            
//            t = position.y - m_avecPosition[idx * m_iNumberHeight + idy + i].y;
//            t = fastSqrt(radius * radius -  t * t);
//            
//            if (t > position.z)
//            {
//                u = t - position.z;
//                utotal += u;
//                m_avecPosition[idx * m_iNumberHeight + idy + i].z -= u;
//            }
//        }
//        // compute force
//        
//        vMultiplyToR(&force, &gravity, -1000.0f * utotal * m_fStep * m_fStep);
//        vAdd(Data->Force, &force);
//    }
//    
//}

void CPhysicsHeightField::Update(float DeltaTime)
{
    const float damping = 0.5f;
    const float speed = 6.0f;
    const float ratio = (speed * speed) / (m_fStep * m_fStep);
    const int lastX = (m_iNumberWidth - 1) * m_iNumberHeight;
    const int beforeLastX = (m_iNumberWidth - 2) * m_iNumberHeight;
    const float maxSlope = 5.0f;
    int ij = 0;
    float offset = 0.0f;
    float maxOffset = 0.0f;
    
    for (int i = 1; i < m_iNumberWidth - 1; i++)
        for (int j = 1; j < m_iNumberHeight - 1; j++)
        {
            ij = i * m_iNumberHeight + j;
            
            m_afVelocity[ij] += (m_avecPosition[ij - m_iNumberHeight].z +
                                m_avecPosition[ij + m_iNumberHeight].z +
                                m_avecPosition[ij - 1].z +
                                m_avecPosition[ij + 1].z -
                                4.0f * m_avecPosition[ij].z) * ratio * DeltaTime;
            
            // working
            /*m_afVelocity[ij] += (m_avecPosition[ij - m_iNumberHeight].z +
                                 m_avecPosition[ij + m_iNumberHeight].z +
                                 m_avecPosition[ij - 1].z +
                                 m_avecPosition[ij + 1].z) * 0.25f - m_avecPosition[ij].z;

            m_afVelocity[ij] *= damping;*/
            
//            offset = (u[i+1,j]+u[i-1,j]+u[i,j+1]+u[i,j-1])/4 – u[i,j]
//            
//            maxOffset = maxSlope * h // independence of resolution h:
//            
//            if (offset > maxOffset) u[i,j] += offset – maxOffset
//            if (offset < -maxOffset) u[i,j] += offset + maxOffset
            
            
        }
    
    for (int i = 1; i < m_iNumberWidth - 1; i++)
        for (int j = 1; j < m_iNumberHeight - 1; j++)
        {
            ij = i * m_iNumberHeight + j;
            
            m_avecPosition[ij].z += m_afVelocity[ij] * DeltaTime;
            // working
            //m_avecPosition[ij].z += m_afVelocity[ij];
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
    
    Globals::GetObjectManager().BroadcastMessage(CMessage(MT_UPDATE_RENDER, static_cast<void*>(m_avecPosition)));
}

void CPhysicsHeightField::Touch(STouchData* Data)
{
    int x = (int)(Data->x / m_fStep) + 1;
    int y = (int)(Data->y / m_fStep) + 1;
    
    m_avecPosition[x * m_iNumberHeight + y].z -= 2.0f;
}


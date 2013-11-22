//
//  CGameObject.cpp
//  dream
//
//  Created by Yermek Garifullanov on 10/7/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#include "CGameObject.h"

#include "IMaterial.h"

CGameObject::CGameObject(E_GAMEOBJECT_ID Id)
{
    m_eId = Id;
    mSetIdentity(&m_matTransformation);
    vSetZero(&m_vecPosition);
    
    vSetZero(&m_vecRotation);
    qSetIdentity(&m_quatRotation);
    mSetIdentity(&m_matRotation);
    
    //vSet(&m_vecPosition, 0.0f, 0.0f, 5.0f);
//    mSetScale(&m_matTransformation, 3.0f, 3.0f, 3.0f); // test
}

E_GAMEOBJECT_ID CGameObject::GetObjectId()
{
    return m_eId;
}

void CGameObject::SetPosition(vector3* NewPosition)
{
    vCopy(&m_vecPosition, NewPosition);
}

vector3* CGameObject::GetPosition()
{
    return &m_vecPosition;
}

void CGameObject::SetRotation(float angle, vector3 *vector)
{
    quaternion temp;
    
    qSetIdentity(&temp);
    vAxisToQuaternion(&temp, vector, angle);
    qMultiply(&m_quatRotation, &temp);

    qNormalize(&m_quatRotation); // important to normalize
}

void CGameObject::SetRotation(quaternion *NewRotation)
{
    qCopy(&m_quatRotation, NewRotation);
}

void CGameObject::SetRotation(vector3* NewRotation)
{
    quaternion q;
    vector3 axis;
    float angle;
    
    vCopy(&m_vecRotation, NewRotation);

    vCopy(&axis, &m_vecRotation);
    angle = vMagnitude(&axis);
    vNormalize(&axis);

    vAxisToQuaternion(&m_quatRotation, &axis, angle);
    //qMultiply(&m_quatRotation, &q);
    qNormalize(&m_quatRotation);
}

vector3* CGameObject::GetRotation()
{
    return &m_vecRotation;
}

void CGameObject::ResetRotaiton()
{
    qSetIdentity(&m_quatRotation);
}

matrix44* CGameObject::GetTransformationMatrix()
{
    matrix44 translate, rotation;
    
    qConvertToMatrix(&rotation, &m_quatRotation);
    mSetTranslation(&translate, m_vecPosition.x, m_vecPosition.y, m_vecPosition.z);
    
    mCopy(&m_matTransformation, &translate);
    mMultiply(&m_matTransformation, &rotation);
    
    //mMultiplyToR(&rotation, &translate, &m_matTransformation);
//    mMultiplyToR(&rotation, &m_matTransformation, &temp);
//    mMultiplyToR(&m_matTransformation, &rotation, &temp);
//    mCopy(&m_matTransformation, &temp);
//    mCopy(&m_matTransformation, &rotation);
    
    return &m_matTransformation;
}


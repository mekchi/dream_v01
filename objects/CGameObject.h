//
//  CGameObject.h
//  dream
//
//  Created by Yermek Garifullanov on 10/7/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef __dream__CGameObject__
#define __dream__CGameObject__

#include <map>

#include "Ids.h"
#include "IComponent.h"
#include "VQMMath.h"


class CObjectManager;

class CGameObject
{
    
public:
    
    CGameObject(E_GAMEOBJECT_ID Id);
    
    E_GAMEOBJECT_ID GetObjectId();

    void SetPosition(vector3* NewPosition);
    void GetPosition(vector3* ToPosition);
    
    void SetRotation(float angle, vector3* vector);
    void SetRotation(quaternion* NewRotation);
    void SetRotation(vector3* NewRotation);
    vector3* GetRotation();
    void ResetRotaiton();
    
    matrix44* GetTransformationMatrix();
    
private:

    // object id
    E_GAMEOBJECT_ID m_eId;
    // object's position
    vector3 m_vecPosition;
    // object's rotaiton
    vector3 m_vecRotation;
    
    quaternion m_quatRotation;

    // object's scale, rotation and translation
    matrix44 m_matTransformation;
    matrix44 m_matRotation;
    
    friend CObjectManager;
    
};

#endif /* defined(__dream__CGameObject__) */

//
//  IComponent.h
//  dream
//
//  Created by Yermek Garifullanov on 10/7/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef __dream__IComponent__
#define __dream__IComponent__

#include "Ids.h"

class CMessage;
class CGameObject;
class CObjectManager;

typedef unsigned int comp_id_type;

class IComponent
{
    
public:
    
    IComponent () {} // : m_opGameObject(nullptr) {}
    virtual ~IComponent() {}
    
    virtual bool Init(void* Property = nullptr) = 0;
    virtual void Deinit() = 0;
    
    virtual E_MESSAGE_RESULT HandleMessage(const CMessage& Message) { return MR_IGNORE; }
    
    virtual E_COMPONENT_ID GetComponentId() = 0;
    virtual E_FAMILY_COMPONENT_ID GetFamilyComponentId() = 0;
    
protected:
    
    CGameObject *m_opObject;
    
    friend CObjectManager;
};

#endif /* defined(__dream__IComponent__) */

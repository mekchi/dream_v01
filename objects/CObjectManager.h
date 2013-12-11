//
//  CObjectManager.h
//  dream
//
//  Created by Yermek Garifullanov on 10/8/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef __dream__CObjectManager__
#define __dream__CObjectManager__

#include <set>
#include <vector>

#include "IComponent.h"
#include "CMessage.h"
#include "CGameObject.h"

typedef unsigned int go_id_type;
typedef IComponent* (*CreateMethod)(void);
typedef void (*DestroyMethod)(IComponent*);

struct SComponentInfo
{
    CreateMethod mCreateMethod;
    DestroyMethod mDestroyMethod;
};

class CObjectManager
{
    
public:
    
    CObjectManager();
    
    bool Init();
    void Deinit();
    
    bool Load();
    
    void RegisterComponent(E_COMPONENT_ID ComponentId, CreateMethod CreateFunc,
                           DestroyMethod DestroyFunc);
    void SubscribeMessage(E_COMPONENT_ID ComponentId, E_MESSAGE_TYPE MessageType);
    void SendMessage(go_id_type Id, const CMessage& Message);
    void BroadcastMessage(const CMessage& Message);
    
//    static CManager* Create ();
    
    CGameObject* CreateObject(E_GAMEOBJECT_ID GameObjectId);
    void DestroyObject(CGameObject *GameObject);
    IComponent* CreateComponent(E_COMPONENT_ID ComponentId);
    void AddComponent(CGameObject* Object, IComponent *Component);
    
private:
    
    //typedef std::map<const go_id_type, IComponent*> go_map_type;
    
    //go_map_type m_mapGameObjects;
    
    // components information
    SComponentInfo m_sComponentInfo[NUM_COMPONENT_ID];
    // subsribed message types by components
    std::set<E_COMPONENT_ID> m_setMsgToComps[NUM_MESSAGE_TYPE];
    // grouped components by thier IDS
    std::set<IComponent*> m_setCompTypeToComps[NUM_COMPONENT_ID];
    // list of game objects with components
//    std::set<IComponent*> m_setObjects[NUM_GAMEOBJECT_IDS];
    std::vector<CGameObject*> m_vectorObjects;
};

#endif /* defined(__dream__CManager__) */

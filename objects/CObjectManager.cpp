//
//  CObjectManager.cpp
//  dream
//
//  Created by Yermek Garifullanov on 10/8/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#include "CObjectManager.h"

#include <algorithm>

#include "Ids.h"
#include "ComponentHeaders.h"

CObjectManager::CObjectManager()
{
    int i = 0;
    
    for (i = 0; i < NUM_COMPONENT_ID; i++)
        m_setCompTypeToComps[i].clear();
    
    for (i = 0; i < NUM_MESSAGE_TYPE; i++)
        m_setMsgToComps[i].clear();
    
    m_vectorObjects.clear();
}

bool CObjectManager::Init()
{
    CProtagonistSphere::Register();
    CEnvironmentHF::Register();
    CLevelRandom::Register();

    CMiracleBox00::Register();
    CMiracleBox01::Register();
    
    return true;
}

void CObjectManager::Deinit()
{
    for (int i = 0; i < NUM_COMPONENT_ID; i++)
    {
        std::set<IComponent*> &comps = m_setCompTypeToComps[i];
        std::set<IComponent*>::iterator comps_iter;
        
        for (comps_iter = comps.begin(); comps_iter != comps.end(); comps_iter++)
        {
            IComponent* comp = *comps_iter;
            
            comp->Deinit();
            m_sComponentInfo[i].mDestroyMethod(comp);
        }
    }
    
    int n = m_vectorObjects.size();
    
    for (int i = 0; i < n; i++)
        delete m_vectorObjects[i];
    
    m_vectorObjects.clear();
}

bool CObjectManager::Load()
{
    SGameProperty propertyHF; //= {32, 48, -16.0f, -24.0f, 0.0f, 1.0f};
    
    vector3 positionP = {0.0f, 0.0f, 3.0f};
    
    propertyHF.NumberWidthHeight = 64;//32;
    propertyHF.NumberHeightHeight = 96;//48;
    propertyHF.OriginX = -16.0f;
    propertyHF.OriginY = -24.0f;
    propertyHF.Level = 0.0f;
    propertyHF.GridStep = 0.5f;//1.0f;
    propertyHF.ProtagonistRadius = 2.0f;
    propertyHF.Left = -11.0f;
    propertyHF.Right = 11.0f;
    propertyHF.Top = 19.0f;
    propertyHF.Bottom = -19.0f;
    propertyHF.WaveRadius = 6.0f;
    propertyHF.WaveAnimationTime = 5.0f;
    
    
    CGameObject* protagonist = CreateObject(GOI_PROTAGONIST);
    IComponent* sphere = CreateComponent(CI_PROTAGONIST_SPHERE);
    
    protagonist->SetPosition(&positionP);
    AddComponent(protagonist, sphere);
    sphere->Init();
    
    CGameObject* environment = CreateObject(GOI_ENVIRONMENT);
    IComponent* hf = CreateComponent(CI_ENVIRONMENT_HEIGHTFIELD);

    AddComponent(environment, hf);
    hf->Init(static_cast<void*>(&propertyHF));

    vector3 protagonistPosition;
    
    CGameObject *level = CreateObject(GOI_LEVEL);
    IComponent *testLevel = CreateComponent(CI_RANDOM_LEVEL);
    
    AddComponent(level, testLevel);
    testLevel->Init(static_cast<void*>(&propertyHF));
    
    protagonist->GetPosition(&protagonistPosition);
    BroadcastMessage(CMessage(MT_PROTAGONIST_POSITION, static_cast<void*>(&protagonistPosition)));
    
    
    
    
//    CGameObject *box = CreateObject(GOI_BOX_COLOR);
//    vector3 positionB = {0.0f, 0.0f, -25.0f};
//    IComponent *star = CreateComponent(CI_MIRACLE_BOX_01);
//    
//    box->SetPosition(&positionB);
//    AddComponent(box, star);
//    
//    star->Init(nullptr);
    

    
    

    return true;
}


void CObjectManager::SendMessage(go_id_type Id, const CMessage &Message)
{
    

}

void CObjectManager::BroadcastMessage(const CMessage &Message)
{
    std::set<E_COMPONENT_ID> &compTypes = m_setMsgToComps[Message.m_eType];
    std::set<E_COMPONENT_ID>::iterator compTypes_iter;
    
    for (compTypes_iter = compTypes.begin(); compTypes_iter != compTypes.end(); compTypes_iter++)
    {
        std::set<IComponent*> &comps = m_setCompTypeToComps[(*compTypes_iter)];
        std::set<IComponent*>::iterator comps_iter;
        
        for (comps_iter = comps.begin(); comps_iter != comps.end(); comps_iter++)
        {
            (*comps_iter)->HandleMessage(Message);
        }
    }
}

void CObjectManager::SubscribeMessage(E_COMPONENT_ID ComponentId, E_MESSAGE_TYPE MessageType)
{
    m_setMsgToComps[MessageType].insert(ComponentId);
}

void CObjectManager::RegisterComponent(E_COMPONENT_ID ComponentId, CreateMethod CreateFunc, DestroyMethod DestroyFunc)
{
    m_sComponentInfo[ComponentId].mCreateMethod = CreateFunc;
    m_sComponentInfo[ComponentId].mDestroyMethod = DestroyFunc;
}

CGameObject* CObjectManager::CreateObject(E_GAMEOBJECT_ID GameObjectId)
{
    CGameObject* object = new CGameObject(GameObjectId);
    
    m_vectorObjects.push_back(object);
    
    return object;
}

void CObjectManager::DestroyObject(CGameObject *GameObject)
{
    std::vector<CGameObject*>::iterator itr;
    
    itr = std::find(m_vectorObjects.begin(), m_vectorObjects.end(), GameObject);
    
    if (itr != m_vectorObjects.end())
    {
        m_vectorObjects.erase(itr);
        
        delete GameObject;
    }
}

IComponent* CObjectManager::CreateComponent(E_COMPONENT_ID ComponentId)
{
    IComponent *comp = m_sComponentInfo[ComponentId].mCreateMethod();
    
    m_setCompTypeToComps[ComponentId].insert(comp);
    
    return comp;
}

void CObjectManager::AddComponent(CGameObject* Object, IComponent *Component)
{
    Component->m_opObject = Object;
}


//
//  CObjectManager.cpp
//  dream
//
//  Created by Yermek Garifullanov on 10/8/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#include "CObjectManager.h"

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
    
    
    CDrawSphere::Register();
    CDrawSurface::Register();
    CPhysicsForce::Register();
    CPhysicsHeightField::Register();
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
    SHeightFieldProperty propertyHF; //= {32, 48, -16.0f, -24.0f, 0.0f, 1.0f};
    
    vector3 positionP = {0.0f, 0.0f, 3.0f};
    
    propertyHF.NumberWidthHeight = 32;
    propertyHF.NumberHeightHeight = 48;
    propertyHF.OriginX = -16.0f;
    propertyHF.OriginY = -24.0f;
    propertyHF.Level = 0.0f;
    propertyHF.GridStep = 1.0f;
    propertyHF.ProtagonistRadius = 1.41f;
    propertyHF.Left = -11.0f;
    propertyHF.Right = 11.0f;
    propertyHF.Top = 19.0f;
    propertyHF.Bottom = -19.0f;
    
//    SHeightFieldProperty propertyHF = {64, 96, -16.0f, -24.0f, 0.0f, 0.5f};
//    SHeightFieldProperty propertyHF = {64, 96};
    
//    CGameObject* protagonist = CreateObject(GOI_PROTAGONIST);
//    CGameObject* environment = CreateObject(GOI_ENVIRONMENT);
//    
//    protagonist->SetPosition(&positionP);
//
//    IComponent* force = CreateComponent(CI_PHYSICS_FORCE);
//    IComponent* sphere = CreateComponent(CI_DRAW_SPHERE);
//
//    IComponent* heightfield = CreateComponent(CI_PHYSICS_HEIGHTFIELD);
//    IComponent* surface = CreateComponent(CI_DRAW_SURFACE);
//    
//    force->Init();
//    sphere->Init();
//    heightfield->Init(static_cast<void*>(&propertyHF));
//    surface->Init(static_cast<void*>(&propertyHF));
//    
//    AddComponent(protagonist, force);
//    AddComponent(environment, heightfield);
//
//    AddComponent(environment, surface);
//    AddComponent(protagonist, sphere);
//    
    
//    CGameObject* box = CreateObject(GOI_BOX_COLOR);
//    vector3 positionB = {0.0f, 4.0f, -2.0f};
//    
//    box->SetPosition(&positionB);
//    
//    IComponent* simple = CreateComponent(CI_MIRACLE_BOX_00);
//    
//    AddComponent(box, simple);
//
//    simple->Init();
    
    
    
    
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

    protagonist->GetPosition(&protagonistPosition);
    BroadcastMessage(CMessage(MT_PROTAGONIST_POSITION, static_cast<void*>(&protagonistPosition)));

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


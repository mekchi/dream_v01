//
//  CMiracleBox.h
//  dream
//
//  Created by Yermek Garifullanov on 11/18/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef __dream__IMiracleBox__
#define __dream__IMiracleBox__

#include "IComponent.h"

#include "Graphics.h"
#include "Vector.h"
#include "CPS_Spark01.h"


class IMiracleBox : public IComponent
{
    
public:
    
    IMiracleBox() {}
    virtual ~IMiracleBox() {}
    
    virtual E_FAMILY_COMPONENT_ID GetFamilyComponentId () { return FCI_MIRACLE_BOX; }
    
};

class CMiracleBox00 : public IMiracleBox
{
    
public:
    
    CMiracleBox00() {}
    virtual ~CMiracleBox00() {}
    
    virtual bool Init(void* Property);
    virtual void Deinit();
    
    virtual E_MESSAGE_RESULT HandleMessage(const CMessage& Message);
    
    virtual E_COMPONENT_ID GetComponentId() { return CI_MIRACLE_BOX_00; }
    
    static void Register();
    static IComponent* Create();
    static void Destroy(IComponent* Component);
    
private:

    void Render();
    
    vector3* m_vecProtagonist;
    int m_iNumFaceVertex;
    GLuint m_gVAO;
    GLuint m_gVBO;
};

class CMiracleBox01 : public IMiracleBox
{
    
public:
    
    CMiracleBox01() {}
    virtual ~CMiracleBox01() {}
    
    virtual bool Init(void* Property);
    virtual void Deinit();
    
    virtual E_MESSAGE_RESULT HandleMessage(const CMessage& Message);
    
    virtual E_COMPONENT_ID GetComponentId() { return CI_MIRACLE_BOX_01; }
    
    static void Register();
    static IComponent* Create();
    static void Destroy(IComponent* Component);
    
private:

    void Update(float DeltaTime);
    void Render();
    
    CPS_Spark01 m_oSpark;
};

#endif /* defined(__dream__CMiracleBox__) */

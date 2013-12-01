//
//  CEngine.cpp
//  dream
//
//  Created by Yermek Garifullanov on 10/8/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#include "CEngine.h"

#include "Globals.h"
#include "CObjectManager.h"
#include "CSoundManager.h"
#include "CShader.h"
#include "IMaterial.h"
#include "VQMMath.h"
#include "Log.h"


CEngine::~CEngine()
{
    
}

bool CEngine::Start(int defaultFB, float Width, float Height, const char* Path)
{
    float w = Width * 0.1f;
    float h = Height * 0.1f;
    
    Globals::SetDefaultFramebuffer(defaultFB);
    Globals::SetResourcePath(Path);
    Globals::SetScreenWidth(w);
    Globals::SetScreenHeight(h);
    
    m_fHalfWidth = w * 0.5f;
    m_fHalfHeight = h * 0.5f;

    if (!CShader::LoadShaders()) return false;
//    if (!IMaterial::FrameBufferInit()) return false;
    
    CObjectManager &manager = Globals::GetObjectManager();

    if (!manager.Init())
    {
        __LOG("Object init failure");
        
        // action
    }
    
    manager.Load();
    
    CSoundManager &sound = Globals::GetSoundManager();
    
    if (!sound.Init())
    {
        __LOG("Sound init failure");
        
        // action
    }
    
    sound.LoadSound(0);
    
    // set up default world transformation
    
    matrix44 modelview, projection;
    
//    matrix44 rotation;
    
    mSetIdentity(&modelview);
//    mSetIdentity(&rotation);
//    mSetRotation(&rotation, 75.0f, 1.0f, 0.0f, 0.0f);

    mSetTranslation(&modelview, 0.0f, 0.0f, -25.0f);
//    mSetTranslation(&modelview, 25.0f, 0.0f, -65.0f);
    
//    mMultiply(&modelview, &rotation);

    //mSetTranslation(&modelview, 0.0f, 5.0f, 0.0f);
    
//    mSetTranslation(&modelview, 0.0f, 0.0f, 2.0f);
    
    Globals::SetDefaultModelviewMatrix(&modelview);
    Globals::ResetModelview();

//    projection = mSetPerspectiveBasic(16.0f, 24.0f, 10.0f, 20.0f);
    projection = mSetPerspective(90.0f * (3.14f / 180.0f), w / h, 0.1f, 200.0f);
//    projection = mSetOrthographic(-160.0f, 160.0f, 240.0f, -240.0f, -50.0f, 50.0f);
//    projection = mSetOrthographic(-16.0f, 16.0f, 24.0f, -24.0f, -5.0f, 5.0f);
    
    
//    projection = mSetOrthographic(-w * 0.5f, w * 0.5f,
//                                  -h * 0.5f, h * 0.5f,
//                                  -10.0f, 10.0f);

//    projection = mSetOrthographic(-w, w,
//                                  -h, h,
//                                  -10.0f, 10.0f);


    Globals::SetProjectionMatrix(&projection);
    CShader::SetProjection();
    //manager.BroadcastMessage(CMessage(MT_UPDATE_PROJECTION));

    
    return true;
}

void CEngine::Stop()
{
    Globals::GetObjectManager().Deinit();
    
    Globals::GetSoundManager().Deinit();
    
    CShader::DeloadShaders();
}

//void CEngine::Run()
//{
//   bool running = true;
//
//    while (running)
//    {
//        Globals::GetObjectManager().BroadcastMessage(CMessage(MT_UPDATE));
//        Globals::GetObjectManager().BroadcastMessage(CMessage(MT_RENDER));
//    }
//}

void CEngine::Update(float DeltaTime)
{
    if (DeltaTime > 0.1f) DeltaTime = 0.1f;
    
    Globals::GetObjectManager().BroadcastMessage(CMessage(MT_UPDATE, static_cast<void*>(&DeltaTime)));
}

void CEngine::Render()
{
    Globals::GetObjectManager().BroadcastMessage(CMessage(MT_RENDER));

//    IMaterial::RenderFrameBuffer();
}

void CEngine::TouchedStart(float x, float y)
{
    vector3 point = {(x * 0.1f)  - m_fHalfWidth,
                    m_fHalfHeight - (y * 0.1f),
                    0.0f};
    
    Globals::GetObjectManager().BroadcastMessage(CMessage(MT_TOUCHED_START, static_cast<void*>(&point)));
}

void CEngine::TouchedStop()
{
    Globals::GetObjectManager().BroadcastMessage(CMessage(MT_TOUCHED_STOP));
}

void CEngine::TouchedMove(float x, float y)
{
    //Globals::GetObjectManager().BroadcastMessage(CMessage());
}

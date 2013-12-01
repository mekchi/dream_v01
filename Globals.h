//
//  Globals.h
//  dream
//
//  Created by Yermek Garifullanov on 10/15/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef dream_Globals_h
#define dream_Globals_h

#include <string>

#include "VQMMath.h"
#include "CObjectManager.h"
#include "CSoundManager.h"
#include "CEngine.h"

class Globals
{
public:
    
    static matrix44* GetModelviewMatrix();
    static matrix44* GetProjectionMatrix();
    static CObjectManager& GetObjectManager();
    static CSoundManager& GetSoundManager();
    static std::string& GetResourcePath();
    static float GetScreenWidth();
    static float GetScreenHeight();
    static int GetDefaultFramebuffer();
    static vector3* GetGravity();
    static void SetTranslation(vector3 *Translate);
    static void ResetModelview();

private:
    
    static bool SetResourcePath(const char* Path);
    static void SetDefaultModelviewMatrix(matrix44* Matrix);
    static void SetProjectionMatrix(matrix44* Matrix);
    static void SetScreenWidth(float Width);
    static void SetScreenHeight(float Height);
    static void SetDefaultFramebuffer(int FB);
    
    static CObjectManager m_oObjectManager;
    static CSoundManager m_oSoundManager;
    
    static std::string m_sResourcePath;
    static matrix44 m_matModelview;
    static matrix44 m_matDefaultModelview;
    static matrix44 m_matProjection;
    static float m_fScreenWidth;
    static float m_fScreenHeight;
    static int m_iDefaultFB;
    
    friend CEngine;
    
};

#endif

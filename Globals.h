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

#include "matrix.h"
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
    static float_t GetScreenWidth();
    static float_t GetScreenHeight();
    static int GetDefaultFramebuffer();
    
private:
    
    static bool SetResourcePath(const char* Path);
    static void SetModelviewMatrix(matrix44* Matrix);
    static void SetProjectionMatrix(matrix44* Matrix);
    static void SetScreenWidth(float_t Width);
    static void SetScreenHeight(float_t Height);
    static void SetDefaultFramebuffer(int FB);
    
    static CObjectManager m_oObjectManager;
    static CSoundManager m_oSoundManager;
    
    static std::string m_sResourcePath;
    static matrix44 m_matModelview;
    static matrix44 m_matProjection;
    static float_t m_fScreenWidth;
    static float_t m_fScreenHeight;
    static int m_iDefaultFB;
    
    friend CEngine;
    
};

#endif

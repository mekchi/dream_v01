//
//  Globals.cpp
//  dream
//
//  Created by Yermek Garifullanov on 10/15/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#include "Globals.h"

std::string Globals::m_sResourcePath("");
matrix44 Globals::m_matDefaultModelview;
matrix44 Globals::m_matModelview;
matrix44 Globals::m_matProjection;
float Globals::m_fScreenWidth = 0.0f;
float Globals::m_fScreenHeight = 0.0f;
CObjectManager Globals::m_oObjectManager;
CSoundManager Globals::m_oSoundManager;
int Globals::m_iDefaultFB = 0;

CObjectManager& Globals::GetObjectManager()
{
    return m_oObjectManager;
}

CSoundManager& Globals::GetSoundManager()
{
    return m_oSoundManager;
}

bool Globals::SetResourcePath(const char* Path)
{
    if (Path != NULL)
    {
        m_sResourcePath = Path;
        
        return true;
    }
    
    return false;
}

std::string& Globals::GetResourcePath()
{
    return m_sResourcePath;
}

void Globals::SetDefaultModelviewMatrix(matrix44 *Matrix)
{
    mCopy(&m_matDefaultModelview, Matrix);
}

matrix44* Globals::GetModelviewMatrix()
{
    return &m_matModelview;
}

void Globals::SetProjectionMatrix(matrix44 *Matrix)
{
    mCopy(&m_matProjection, Matrix);
}

matrix44* Globals::GetProjectionMatrix()
{
    return &m_matProjection;
}

float Globals::GetScreenWidth()
{
    return m_fScreenWidth;
}

float Globals::GetScreenHeight()
{
    return m_fScreenHeight;
}

int Globals::GetDefaultFramebuffer()
{
    return m_iDefaultFB;
}

vector3* Globals::GetGravity()
{
    static vector3 gravity = {0.0f, 0.0f, -9.8f};
    
    return &gravity;
}

void Globals::SetTranslation(vector3 *Translate)
{
    matrix44 m;
    
    mCopy(&m_matModelview, &m_matDefaultModelview);
    mSetIdentity(&m);
    mSetTranslation(&m, Translate->x, Translate->y, Translate->z);
    mMultiply(&m_matModelview, &m);
}

void Globals::ResetModelview()
{
    mCopy(&m_matModelview, &m_matDefaultModelview);
}

void Globals::SetScreenWidth(float Width)
{
    m_fScreenWidth = Width;
}

void Globals::SetScreenHeight(float Height)
{
    m_fScreenHeight = Height;
}

void Globals::SetDefaultFramebuffer(int FB)
{
    m_iDefaultFB = FB;
}
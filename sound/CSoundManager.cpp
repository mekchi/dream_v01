//
//  CSoundManager.cpp
//  dream
//
//  Created by Yermek Garifullanov on 11/20/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#include "CSoundManager.h"

#include <OpenAL/oalMacOSX_OALExtensions.h>

#include "stb_vorbis.h"
#include "LoadTools.h"
#include "Log.h"

CSoundManager::CSoundManager()
{
    m_opDevice = nullptr;
    m_opContext = nullptr;
}

CSoundManager::~CSoundManager()
{
    
}

bool CSoundManager::Init()
{
    /* Open and initialize a device with default settings */
    m_opDevice = alcOpenDevice(nullptr);

    if(!m_opDevice)
    {
        __LOG("Sound : Faild to open a device;");

        return false;
    }
    
    m_opContext = alcCreateContext(m_opDevice, nullptr);
    
    if(!m_opContext || alcMakeContextCurrent(m_opContext) == ALC_FALSE)
    {
        if(m_opContext != nullptr) alcDestroyContext(m_opContext);

        alcCloseDevice(m_opDevice);
        
        __LOG("Sound : Faild to set a context;");

        return false;
    }
    
    
    
//    alIsExtensionPresent(<#const ALchar *extname#>)
    
    
    return true;
}

void CSoundManager::Deinit()
{
    alcMakeContextCurrent(nullptr);
    
    if (m_opContext) alcDestroyContext(m_opContext);
    if (m_opDevice) alcCloseDevice(m_opDevice);
}

bool CSoundManager::LoadSound(int Id)
{
    int sizeFullData = 0, channels = 0;
    unsigned char *fullData = nullptr;
    short *pureData = nullptr;
    
    if (!LoadFileBytes("music.ogg", &fullData, &sizeFullData))
    {
        __LOG("Faild to load sound");
        
        return false;
    }

    if (-1 == stb_vorbis_decode_memory(fullData, sizeFullData, &channels, &pureData))
    {
        __LOG("Faild to decode sound data");
        
        delete fullData;
        
        return false;
    }
    
    alGenBuffers(1, &m_aBuffer);
    alBufferData(m_aBuffer, AL_FORMAT_STEREO16, static_cast<ALvoid*>(pureData), sizeFullData * 2, 44100);
    alGenSources(1, &m_aSource);
    alSourcei(m_aSource, AL_BUFFER, m_aBuffer);
    alSourcei(m_aSource, AL_LOOPING, AL_TRUE);

    delete fullData;
    free(pureData);
    
    return true;
}


void CSoundManager::PlaySound(int Id)
{
    alSourcePlay(m_aSource);
}


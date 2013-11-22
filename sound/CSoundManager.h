//
//  CSoundManager.h
//  dream
//
//  Created by Yermek Garifullanov on 11/20/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef __dream__CSoundManager__
#define __dream__CSoundManager__

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

class CSoundManager
{
    
public:
    
    CSoundManager();
    virtual ~CSoundManager();
    
    bool Init();
    void Deinit();
    
    bool LoadSound(int Id);
    void PlaySound(int Id);
    
private:
    
    ALCdevice* m_opDevice;
    ALCcontext* m_opContext;
    
    ALuint m_aBuffer;
    ALuint m_aSource;
    
};

#endif /* defined(__dream__CSoundManager__) */

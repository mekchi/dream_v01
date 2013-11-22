//
//  CLoaderMek.h
//  dream
//
//  Created by Yermek Garifullanov on 10/27/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef __dream__CLoaderMek__
#define __dream__CLoaderMek__

#include "Types.h"

class CLoaderMek
{
    
public:
    CLoaderMek();
    ~CLoaderMek();
    
    bool Load(const char* FileName);
    int GetNumberOfFace();
    int GetSize();
    float_t* GetData();
    
private:
    
    int m_iNumFace;
    
    float_t* m_avecVertexNormalUV;
};

#endif /* defined(__dream__CLoaderMek__) */

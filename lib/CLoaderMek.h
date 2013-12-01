//
//  CLoaderMek.h
//  dream
//
//  Created by Yermek Garifullanov on 10/27/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef __dream__CLoaderMek__
#define __dream__CLoaderMek__

class CLoaderMek
{
    
public:
    CLoaderMek();
    ~CLoaderMek();
    
    bool Load(const char* FileName);
    int GetNumberOfFace();
    int GetSize();
    float* GetData();
    
private:
    
    int m_iNumFace;
    
    float* m_avecVertexNormalUV;
};

#endif /* defined(__dream__CLoaderMek__) */

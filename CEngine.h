//
//  CEngine.h
//  dream
//
//  Created by Yermek Garifullanov on 10/8/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef __dream__CEngine__
#define __dream__CEngine__

#include "Types.h"
#include "DataTypes.h"

class CEngine
{
    
public:
    
    ~CEngine();
    
    bool Start(int defaultFB, float Width, float Height, const char* Path);
    void Stop();
//    void Run();
    void Update(float DeltaTime);
    void Render();
    
    void TouchedStart(float x, float y);
    void TouchedStop();
    void TouchedMove(float x, float y);
    
private:
    
    float_t m_fHalfWidth;
    float_t m_fHalfHeight;
    
    //SShaderUniformModelview m_sWorldTransformation;
    
};

#endif /* defined(__dream__CEngine__) */

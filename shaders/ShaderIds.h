//
//  ShaderIds.h
//  dream
//
//  Created by Yermek Garifullanov on 10/21/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef dream_ShaderIds_h
#define dream_ShaderIds_h

enum E_SHADER_ATTR_LOCATION
{
    SAL_POSITION = 0,
    SAL_NORMAL,
    SAL_TEXTURE_COORD
};

enum E_SHADER_UNIFORM_LOCATION
{
    SUL_PROJECTION = 0,
    SUL_MODELVIEW,
    SUL_NORMALMATRIX,
    SUL_TEXTURE0,
    SUL_TEXTURE1,
    SUL_LIGHTPOSITION,
    SUL_FLOAT0,
    SUL_COLOR0,
};

enum E_SHADER_TYPE
{
    ST_FRAME = 0,
    ST_DEFAULT,
    ST_TEXTURE,
    ST_VERTICAL_GLOW,
    ST_HORIZONTAL_GLOW,
    ST_BLEND,
    ST_WAVE,
    ST_HF,
    ST_PARTICLE,
    NUM_SHADER_TYPE
};

#endif

//
//  DataTypes.h
//  dream
//
//  Created by Yermek Garifullanov on 10/10/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef dream_DataTypes_h
#define dream_DataTypes_h

#include <string>

#include "VQMMath.h"
#include "CGameObject.h"

// Message data types

struct SShaderUniformModelview
{
    matrix44 ModelView;
    matrix44 NormalMatrix;
};
typedef SShaderUniformModelview SShaderUniformModelview;

typedef matrix44 SShaderUniformProjection;

typedef vector3 STouchData;

struct SObjectData
{
    vector3 position;
    float Radius;
    vector3* Force;
    vector3* Torque;
};

typedef SObjectData SObjectData;

// Init data types

typedef struct
{
    // grid definition
    int NumberWidthHeight;
    int NumberHeightHeight;

    float OriginX; // origin of grid
    float OriginY;
    float Level; // z coordinate
    float GridStep;
    
    // borders
    float Left;
    float Right;
    float Top;
    float Bottom;
 
    // protagonist
    float ProtagonistRadius;
    
    // wave
    float WaveRadius;
    float WaveAnimationTime;
    
} SGameProperty;

// Material data

typedef struct
{
    std::string TextureFileName;
} SMaterialData;

typedef struct
{
    matrix44* ModelView;
    vector3* LightPosition;
    float* FloatParams;
    int* IntParams;
    
} SMaterialRenderData;

// Particle system data

typedef struct
{
    int fps;
    vector3 *Position;
    float LifeTime;
    vector3 *Color;
    
} SPS_Property;


#endif

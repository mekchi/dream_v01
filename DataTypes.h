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

#include "Types.h"
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
    int NumberWidthHeight;
    int NumberHeightHeight;
    
    float_t OriginX;
    float_t OriginY;
    float_t Level; // z coordinate
    float_t GridStep;
    
    // borders
    float_t Left;
    float_t Right;
    float_t Top;
    float_t Bottom;
    
    float_t ProtagonistRadius;
} SHeightFieldProperty;

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


#endif

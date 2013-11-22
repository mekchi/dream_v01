//
//  Ids.h
//  dream
//
//  Created by Yermek Garifullanov on 10/7/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef dream_Ids_h
#define dream_Ids_h

// --- GAMEOBJECT ---

enum E_GAMEOBJECT_ID
{
    GOI_PROTAGONIST = 0,
    GOI_ENVIRONMENT,
    GOI_BOX_COLOR,
    GOI_BOX_SOUND,
    GOI_BOX_WORD,
    NUM_GAMEOBJECT_ID
};

enum E_OBJECT_STATUS
{
    OS_IDLE = 0,
    OS_MOVE,
    OS_ACT_TOUCH
};

// --- GAMEOBJECT ---

// --- COMPONENT ---

enum E_FAMILY_COMPONENT_ID
{
    FCI_DRAW_OBJECT = 0,
    FCI_PHYSICS,
    FCI_MIRACLE_BOX
};

// order is important

enum E_COMPONENT_ID
{
//    CI_TRANSFORMATION = 0,

    CI_DRAW_SPHERE = 0,
    CI_DRAW_SURFACE,
    CI_PHYSICS_FORCE,
    CI_PHYSICS_HEIGHTFIELD,
    CI_MIRACLE_BOX_00,
    CI_MIRACLE_BOX_01,
    
    
//    CI_HEIGHTFIELD,
//    CI_HEIGHTFIELDQUAD,
//    CI_COLLISION_WEIGHT,
    NUM_COMPONENT_ID
};

// --- COMPONENT ---

// --- MESSAGE IDS ---

enum E_MESSAGE_RESULT
{
    MR_SUCCESS = 0,
    MR_FAILD,
    MR_ERROR,
    MR_IGNORE
};

enum E_MESSAGE_TYPE
{
    MT_UPDATE = 0,
    MT_RENDER,
    MT_UPDATE_RENDER,
    MT_PROTAGONIST_COMPUTE_FORCE,
    MT_PROTAGONIST_POSITION,
    MT_UPDATE_PROJECTION,
    MT_MOVE,
    MT_TOUCHED_OBJECT,
    MT_TOUCHED_START,
    MT_TOUCHED_MOVE,
    MT_TOUCHED_STOP,
    MT_SPACE,
    MT_PROTAGONIST_TOUCH,
    NUM_MESSAGE_TYPE
};

// --- MESSAGE IDS ---

#endif

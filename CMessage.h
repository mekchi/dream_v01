//
//  CMessage.h
//  dream
//
//  Created by Yermek Garifullanov on 10/7/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef __dream__CMessage__
#define __dream__CMessage__

#include "Ids.h"

class CMessage
{
    
public:
    
    CMessage () {}
    CMessage (E_MESSAGE_TYPE Type) : m_eType (Type), m_vpData (nullptr) {}
    CMessage (E_MESSAGE_TYPE Type, void* Data) : m_eType (Type), m_vpData (Data) {}

    
    E_MESSAGE_TYPE m_eType;
    void* m_vpData;
    
};

#endif /* defined(__dream__CMessage__) */

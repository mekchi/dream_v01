//
//  CLoaderMek.cpp
//  dream
//
//  Created by Yermek Garifullanov on 10/27/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#include "CLoaderMek.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "Types.h"
#include "Log.h"
#include "Globals.h"

CLoaderMek::CLoaderMek()
{
    m_iNumFace = 0;
    m_avecVertexNormalUV = NULL;
}

CLoaderMek::~CLoaderMek()
{
    if (m_avecVertexNormalUV == NULL)
    {
        delete m_avecVertexNormalUV;
    }
}

bool CLoaderMek::Load(const char* FileName)
{
    FILE *file = NULL;
    size_t size = 0;
    char* buffer = NULL;
    char* offset = NULL;
    
    std::string path = Globals::GetResourcePath() + "/" + FileName;
    
    file = fopen(path.c_str(), "rb");
    
    if (file == NULL)
    {
        __LOG("Faild to open Mek file!");
        
        return false;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    buffer = (char*)malloc(size + 1);
    memset(buffer, 0, size + 1);
    
    if (buffer == NULL)
    {
        __LOG("Faild to create bufffer(Mek file)!");
        fclose(file);
        
        return false;
    }
    
    if (size != fread(buffer, 1, size, file))
    {
        __LOG("Faild to read Mek file!");
        free(buffer);
        fclose(file);
        
        return false;
    }
    
    offset = buffer;
    
    char cVersion = 0;
    int iVersion = 0;

    memcpy(&iVersion, offset, 4);
    offset += 4;
    memcpy(&cVersion, offset, 1);
    offset++;
    
    if (iVersion == 1 && cVersion == 'a')
    {
        memcpy(&m_iNumFace, offset, 4);
        offset += 4;
        // formula: size = NumFace * 3(points) * 4(float size) * (3 + 3 + 2)(position, normal and UV)
        float dataSize = m_iNumFace * 3 * 4 * (3 + 3 + 2);
        if ((size - 9) == dataSize)
        {
            // formula: n = NumFace * 3(points) * (3 + 3 + 2)(position, normal and UV
            int n = m_iNumFace * 3 * (3 + 3 + 2);
            m_avecVertexNormalUV = new float_t[n];
            memcpy(m_avecVertexNormalUV, offset, dataSize);
            
            free(buffer);
            fclose(file);
            
            return true;
        }
    }
    
    free(buffer);
    fclose(file);
    
    return false;
}

int CLoaderMek::GetNumberOfFace()
{
    return m_iNumFace;
}

int CLoaderMek::GetSize()
{
    return m_iNumFace * 3 * 4 * (3 + 3 + 2);
}

float_t* CLoaderMek::GetData()
{
    return m_avecVertexNormalUV;
}

//
//  IMaterial.cpp
//  dream
//
//  Created by Yermek Garifullanov on 10/8/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#include "IMaterial.h"

#include <assert.h>

#include "Log.h"
#include "CShader.h"
#include "Globals.h"
#include "CObjectManager.h"
#include "VQMMath.h"
#include "LoadTools.h"

GLuint IMaterial::m_gMainTexFrameBuffer = 0;
GLuint IMaterial::m_gFB = 0;
GLuint IMaterial::m_gRBDepth = 0;
GLuint IMaterial::m_gVAO = 0;
GLuint IMaterial::m_gVBO = 0;

GLuint IMaterial::m_gRB = 0;

GLfloat IMaterial::m_fViewportWidth = 0.0f;
GLfloat IMaterial::m_fViewportHeight = 0.0f;

// simple quad
// vertex and texture coordinates
static float_t quad[30] =
{
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
    1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
    -1.0f, 1.0f, 0.0f,   0.0f, 1.0f
};


bool IMaterial::FrameBufferInit()
{
    GLint viewport[4];
    
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    m_fViewportWidth = viewport[2];
    m_fViewportHeight = viewport[3];
    
    glGenFramebuffers(1, &m_gFB);
    glBindFramebuffer(GL_FRAMEBUFFER, m_gFB);
    
    glGenTextures(1, &m_gMainTexFrameBuffer);
    glBindTexture(GL_TEXTURE_2D, m_gMainTexFrameBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_fViewportWidth, m_fViewportHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gMainTexFrameBuffer, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glGenRenderbuffers(1, &m_gRB);
    glBindRenderbuffer(GL_RENDERBUFFER, m_gRB);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, m_fViewportWidth, m_fViewportHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_gRB);
    
    glGenRenderbuffers(1, &m_gRBDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, m_gRBDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_fViewportWidth, m_fViewportHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gRBDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    

//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
//    glDepthMask(GL_TRUE);
//    glDepthRangef(0.0f, 1.0f);
//    glClearDepthf(1.0f);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        __LOG("Faild to create frame buffer");
        
        return false;
    }
    
    glGenVertexArraysOES(1, &m_gVAO);
    glBindVertexArrayOES(m_gVAO);
    
    glGenBuffers(1, &m_gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(SAL_POSITION);
    glVertexAttribPointer(SAL_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(float_t) * 5, 0);
    glEnableVertexAttribArray(SAL_TEXTURE_COORD);
    glVertexAttribPointer(SAL_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(float_t) * 5, (char*)NULL + (sizeof(float_t) * 3));
    
    glBindVertexArrayOES(0);
    

    glBindFramebuffer(GL_FRAMEBUFFER, Globals::GetDefaultFramebuffer());
    //glBindFramebuffer(GL_FRAMEBUFFER, m_gFB);

    return true;
}

void IMaterial::FrameBufferDeinit()
{
    if (m_gMainTexFrameBuffer)
        glDeleteTextures(1, &m_gMainTexFrameBuffer);
    if (m_gFB)
        glDeleteFramebuffers(1, &m_gFB);
    if (m_gRBDepth)
        glDeleteRenderbuffers(1, &m_gRBDepth);
    if (m_gVAO)
        glDeleteVertexArraysOES(1, &m_gVAO);
    if (m_gVBO)
        glDeleteBuffers(1, &m_gVBO);
}

void IMaterial::RenderFrameBuffer()
{
    glBindTexture(GL_TEXTURE_2D, m_gMainTexFrameBuffer);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 640, 960, 0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, Globals::GetDefaultFramebuffer());
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(CShader::GetShader(ST_FRAME)->GetProgram());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gMainTexFrameBuffer);
    glUniform1i(CShader::GetShader(ST_FRAME)->GetUnivormLocation(SUL_TEXTURE0), 0);
    
    RenderQuad();
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_gFB);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void IMaterial::RenderQuad()
{
    glBindVertexArrayOES(m_gVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

GLint IMaterial::GetScreenTexture()
{
    glBindTexture(GL_TEXTURE_2D, m_gMainTexFrameBuffer);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, m_fViewportWidth, m_fViewportHeight, 0);
    
    return m_gMainTexFrameBuffer;
}

CMaterialDefault::CMaterialDefault(IDrawObject* DrawObject) : IMaterial(DrawObject)
{
    
}

CMaterialDefault::~CMaterialDefault()
{
    
}

bool CMaterialDefault::Init(SMaterialData* Data)
{
    
    
    return true;
}

void CMaterialDefault::Deinit()
{
    
}

void CMaterialDefault::Render(SMaterialRenderData* Data)
{
    matrix44 temp, invertTemp;
    
    mMultiplyToR(Globals::GetModelviewMatrix(), Data->ModelView, &temp);
    
    mInvertToR(&temp, &invertTemp);
    
    glUseProgram(CShader::GetShader(ST_DEFAULT)->GetProgram());
    
    glUniformMatrix4fv(CShader::GetShader(ST_DEFAULT)->GetUnivormLocation(SUL_MODELVIEW), 1, GL_FALSE, temp.m);
    glUniformMatrix4fv(CShader::GetShader(ST_DEFAULT)->GetUnivormLocation(SUL_NORMALMATRIX), 1, GL_FALSE, invertTemp.m);
    glUniform3fv(CShader::GetShader(ST_DEFAULT)->GetUnivormLocation(SUL_LIGHTPOSITION), 1, Data->LightPosition->v);

    
    m_opDrawObject->Render();
}

// ---- CMaterialTexture ----

CMaterialTexture::CMaterialTexture(IDrawObject* DrawObject) : IMaterial(DrawObject)
{
    m_gTexture = 0;
}

CMaterialTexture::~CMaterialTexture()
{
    
}

bool CMaterialTexture::Init(SMaterialData* Data)
{
    if (Data == nullptr)
        return false;
    
    glGenTextures(1, &m_gTexture);
    
    glBindTexture(GL_TEXTURE_2D, m_gTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
//    if (!loadBmpImage(Data->TextureFileName.c_str()))
//        return false;
    
    return true;
}

void CMaterialTexture::Deinit()
{
    if (m_gTexture)
        glDeleteTextures(1, &m_gTexture);
}

void CMaterialTexture::Render(SMaterialRenderData* Data)
{
    matrix44 temp, invertTemp;
    
    mMultiplyToR(Globals::GetModelviewMatrix(), Data->ModelView, &temp);
    mInvertToR(&temp, &invertTemp);
    
    glUseProgram(CShader::GetShader(ST_TEXTURE)->GetProgram());
    
    glUniformMatrix4fv(CShader::GetShader(ST_TEXTURE)->GetUnivormLocation(SUL_MODELVIEW), 1, GL_FALSE, temp.m);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gTexture);

    glUniform1i(CShader::GetShader(ST_TEXTURE)->GetUnivormLocation(SUL_TEXTURE0), 0);
    
    m_opDrawObject->Render();
    
//    glBindTexture(GL_TEXTURE_2D, 0);
}



CMaterialGlow::CMaterialGlow(IDrawObject* DrawObject) : IMaterial(DrawObject)
{
    

}

CMaterialGlow::~CMaterialGlow()
{
    
}

bool CMaterialGlow::Init(SMaterialData* Data)
{
    if (Data == nullptr)
        return false;
    
//    static float_t quad[30] =
//    {
//        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
//        1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
//        1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
//        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
//        1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
//        -1.0f, 1.0f, 0.0f,   0.0f, 1.0f
//    };
    
    GLint viewport[4];
    
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    m_gTextureWidth = viewport[2];
    m_gTextureHeight = viewport[3];
//    
//    glGenTextures(1, &m_gTexture);
//    
//    glBindTexture(GL_TEXTURE_2D, m_gTexture);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    
//    if (!loadBmpImage(data->TextureFileName.c_str()))
//        return false;

    
    
    glGenFramebuffers(1, &m_gFB);
    glGenRenderbuffers(1, &m_gRB);
    glGenTextures(2, m_gTexture);

    glBindTexture(GL_TEXTURE_2D, m_gTexture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_gTextureWidth, m_gTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glBindTexture(GL_TEXTURE_2D, m_gTexture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_gTextureWidth, m_gTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glBindRenderbuffer(GL_RENDERBUFFER, m_gRB);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, m_gTextureWidth, m_gTextureHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_gFB);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_gRB);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        __LOG("Faild to create frame buffer");
        
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, Globals::GetDefaultFramebuffer());
    
    glGenVertexArraysOES(1, &m_gVAO);
    glBindVertexArrayOES(m_gVAO);
    
    glGenBuffers(1, &m_gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_gVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(SAL_POSITION);
    glVertexAttribPointer(SAL_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(float_t) * 5, 0);
    glEnableVertexAttribArray(SAL_TEXTURE_COORD);
    glVertexAttribPointer(SAL_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(float_t) * 5, (char*)NULL + (sizeof(float_t) * 3));
    
    glBindVertexArrayOES(0);

    return true;
}

void CMaterialGlow::Deinit()
{
    
}

void CMaterialGlow::Render(SMaterialRenderData* Data)
{
    matrix44 temp, invertTemp;
    
    mMultiplyToR(Globals::GetModelviewMatrix(), Data->ModelView, &temp);
    mInvertToR(&temp, &invertTemp);

    // basic rendering
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_gFB);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(CShader::GetShader(ST_DEFAULT)->GetProgram());
    glUniformMatrix4fv(CShader::GetShader(ST_DEFAULT)->GetUnivormLocation(SUL_MODELVIEW), 1, GL_FALSE, temp.m);
    glUniformMatrix4fv(CShader::GetShader(ST_DEFAULT)->GetUnivormLocation(SUL_NORMALMATRIX), 1, GL_FALSE, invertTemp.m);
    
    m_opDrawObject->Render();
    
    glBindTexture(GL_TEXTURE_2D, m_gTexture[0]);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, m_gTextureWidth, m_gTextureHeight, 0);

    // vertical blur
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gTexture[0]);
    
    glUseProgram(CShader::GetShader(ST_VERTICAL_GLOW)->GetProgram());
    glUniform1i(CShader::GetShader(ST_VERTICAL_GLOW)->GetUnivormLocation(SUL_TEXTURE0), 0);

    glBindVertexArrayOES(m_gVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindTexture(GL_TEXTURE_2D, m_gTexture[1]);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, m_gTextureWidth, m_gTextureHeight, 0);

    // horizontal blur

    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, m_gTexture[1]);
    
    glUseProgram(CShader::GetShader(ST_HORIZONTAL_GLOW)->GetProgram());
    glUniform1i(CShader::GetShader(ST_HORIZONTAL_GLOW)->GetUnivormLocation(SUL_TEXTURE0), 1);

    glBindVertexArrayOES(m_gVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    //glBindTexture(GL_TEXTURE_2D, m_gTexture[1]);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, m_gTextureWidth, m_gTextureHeight, 0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, Globals::GetDefaultFramebuffer());

//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    
//    glUseProgram(CShader::GetShader(ST_TEXTURE)->GetProgram());
//
//    glUniformMatrix4fv(CShader::GetShader(ST_TEXTURE)->GetUnivormLocation(SUL_MODELVIEW), 1, GL_FALSE, temp.m);
//
//    glActiveTexture(GL_TEXTURE0 + 1);
//    glBindTexture(GL_TEXTURE_2D, m_gTexture[1]);
//
//    glUniform1i(CShader::GetShader(ST_TEXTURE)->GetUnivormLocation(SUL_TEXTURE0), 1);
//
//    glBindVertexArrayOES(m_gVAO[0]);
//    
//    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(CShader::GetShader(ST_BLEND)->GetProgram());

    glUniform1i(CShader::GetShader(ST_TEXTURE)->GetUnivormLocation(SUL_TEXTURE0), 1);
    glUniform1i(CShader::GetShader(ST_TEXTURE)->GetUnivormLocation(SUL_TEXTURE1), 2);

    glBindVertexArrayOES(m_gVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);    
}


CMaterialWave::CMaterialWave() : IMaterial(nullptr)
{
    
}

CMaterialWave::~CMaterialWave()
{
    
}

bool CMaterialWave::Init(SMaterialData* Data)
{
    if (Data == nullptr)
        return false;
    
    glGenTextures(1, &m_gTexture);
    
    glBindTexture(GL_TEXTURE_2D, m_gTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
//    if (!loadBmpImage(Data->TextureFileName.c_str()))
//        return false;
    
    return true;
}

void CMaterialWave::Deinit()
{
    
}

void CMaterialWave::Render(SMaterialRenderData* Data)
{
    GLint tex0 = GetScreenTexture();
    
    glUseProgram(CShader::GetShader(ST_WAVE)->GetProgram());
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex0);
    glUniform1i(CShader::GetShader(ST_WAVE)->GetUnivormLocation(SUL_TEXTURE0), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_gTexture);
    glUniform1i(CShader::GetShader(ST_WAVE)->GetUnivormLocation(SUL_TEXTURE1), 1);
//    glUniform1fv(CShader::GetShader(ST_WAVE)->GetUnivormLocation(SUL_FLOATV), 5, Data->Params);
    
    
//    glUniformMatrix4fv(CShader::GetShader(ST_WAVE)->GetUnivormLocation(SUL_TEXTURE1), 1, GL_FALSE, invertTemp.m);
    
    RenderQuad();
}

// CMaterialHeightField


CMaterialHeightField::CMaterialHeightField(IDrawObject* DrawObject) : IMaterial(DrawObject)
{
    
}

CMaterialHeightField::~CMaterialHeightField()
{
    
}

bool CMaterialHeightField::Init(SMaterialData* Data)
{
    if (Data == nullptr)
        return false;
    
    glGenTextures(1, &m_gTexture);
    
    glBindTexture(GL_TEXTURE_2D, m_gTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
//    if (!loadBmpImage(Data->TextureFileName.c_str()))
//        return false;
    
    return true;
}

void CMaterialHeightField::Deinit()
{
    
}

void CMaterialHeightField::Render(SMaterialRenderData* Data)
{
    matrix44 temp, invertTemp;
    
    mMultiplyToR(Globals::GetModelviewMatrix(), Data->ModelView, &temp);
    
    mInvertToR(&temp, &invertTemp);
    
    glUseProgram(CShader::GetShader(ST_HF)->GetProgram());
    
    glUniformMatrix4fv(CShader::GetShader(ST_HF)->GetUnivormLocation(SUL_MODELVIEW), 1, GL_FALSE, temp.m);
    glUniformMatrix4fv(CShader::GetShader(ST_HF)->GetUnivormLocation(SUL_NORMALMATRIX), 1, GL_FALSE, invertTemp.m);
    glUniform3fv(CShader::GetShader(ST_HF)->GetUnivormLocation(SUL_LIGHTPOSITION), 1, Data->LightPosition->v);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gTexture);
    glUniform1i(CShader::GetShader(ST_HF)->GetUnivormLocation(SUL_TEXTURE0), 0);
    
    glUniform1f(CShader::GetShader(ST_HF)->GetUnivormLocation(SUL_FLOAT0), *(Data->FloatParams));
    
    m_opDrawObject->Render();
    
//    glBindTexture(GL_TEXTURE_2D, 0);
}




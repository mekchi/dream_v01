//
//  IShader.cpp
//  dream
//
//  Created by Yermek Garifullanov on 10/21/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#include "CShader.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>

#include "Log.h"
#include "Globals.h"

GLuint CShader::m_arrayShaderProgram[NUM_SHADER_TYPE];
CShader* CShader::m_arrayShaderPointer[NUM_SHADER_TYPE];

bool CShader::LoadShaders()
{
    m_arrayShaderPointer[ST_FRAME] = new CShaderFrame();
    m_arrayShaderPointer[ST_DEFAULT] = new CShaderDefault();
    
    m_arrayShaderPointer[ST_TEXTURE] = new CShaderTexture(); // for test
    
    m_arrayShaderPointer[ST_VERTICAL_GLOW] = new CShaderVerticalGlow();
    m_arrayShaderPointer[ST_HORIZONTAL_GLOW] = new CShaderHorizontalGlow();
    m_arrayShaderPointer[ST_BLEND] = new CShaderBlend();
    m_arrayShaderPointer[ST_WAVE] = new CShaderWave();
    m_arrayShaderPointer[ST_HF] = new CShaderHeightField();
    m_arrayShaderPointer[ST_PARTICLE] = new CShaderParticle();
    
    for (int i = 0; i < NUM_SHADER_TYPE; i++)
    {
        if (m_arrayShaderPointer[i]->Init())
        {
            m_arrayShaderProgram[i] = m_arrayShaderPointer[i]->GetProgram();
        }
        else
        {
            __LOG("Faild to load shaders!");
            
            return false;
        }
    }
    
    return true;
}

void CShader::DeloadShaders()
{
    for (int i = 0; i < NUM_SHADER_TYPE; i++)
    {
        m_arrayShaderPointer[i]->Deinit();
        delete m_arrayShaderPointer[i];
    }
}

CShader* CShader::GetShader(E_SHADER_TYPE Type)
{
    return m_arrayShaderPointer[Type];
}

void CShader::SetProjection()
{
    for (int i = 0; i < NUM_SHADER_TYPE; i++)
    {
        glUseProgram(m_arrayShaderPointer[i]->m_gProgram);
        
        GLint projection = m_arrayShaderPointer[i]->GetUnivormLocation(SUL_PROJECTION);
        
        if (projection >= 0)
            glUniformMatrix4fv(projection, 1, GL_FALSE, Globals::GetProjectionMatrix()->m);
            
    }
}

GLuint CShader::CreateProgram(const char *vtFileName, const char* fgFileName,
                               GLuint *vtShader, GLuint *fgShader)
{
    GLuint program;
    
    // Create shader program.
    program = glCreateProgram();
    
    // Create and compile vertex shader.
    *vtShader = CompileShader(vtFileName, GL_VERTEX_SHADER);
    if (!(*vtShader))
    {
        __LOGARG("---%s---", vtFileName);
        glDeleteProgram(program);
        
        return 0;
    }
    // Create and compile fragment shader.
    *fgShader = CompileShader(fgFileName, GL_FRAGMENT_SHADER);
    if (!(*fgShader))
    {
        __LOGARG("---%s---", fgFileName);
        glDeleteShader(*vtShader);
        glDeleteProgram(program);
        
        return 0;
    }
    // Attach vertex shader to program.
    glAttachShader(program, *vtShader);
    
    // Attach fragment shader to program.
    glAttachShader(program, *fgShader);
    
    return program;
}

void CShader::DestroyProgram(GLuint program, GLuint vtShader, GLuint fgShader)
{
    if (program)
    {
        if (vtShader)
        {
            glDetachShader(program, vtShader);
            glDeleteShader(vtShader);
        }
        if (fgShader)
        {
            glDetachShader(program, fgShader);
            glDeleteShader(fgShader);
        }
        glDeleteProgram(program);
    }
}

int CShader::LinkProgram(GLuint program, GLuint vtShader, GLuint fgShader)
{
    GLint status;
    
#ifdef DEBUG
    GLint bufSize = 0;
    GLchar *buf = NULL;
#endif
    
    glLinkProgram(program);
    
#ifdef DEBUG
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
    if (bufSize > 0)
    {
        buf = (GLchar*)malloc(bufSize);
        glGetProgramInfoLog(program, bufSize, &bufSize, buf);
        __LOGARG("Program link log:\n%s", buf);
        free(buf);
    }
#endif
    
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == 0)
    {
        DestroyProgram(program, vtShader, fgShader);
        
        return 0;
    }
    
    return 1;
}


GLuint CShader::CompileShader (const char *filename, GLenum type)
{
    GLuint shader;
    GLuint status = 0;
    size_t size = 0;
    FILE *file = NULL;
    GLchar *buffer = NULL;
    std::string path = Globals::GetResourcePath() + "/" + filename;
    
    file = fopen (path.c_str(), "rt");
    
    if (file == NULL)
    {
        __LOG("Faild to open shader file!");
        
        return 0;
    }
    
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    buffer = (GLchar*)malloc(size + 1);
    memset(buffer, 0, size + 1);
    
    if (buffer == NULL)
    {
        __LOG("Faild to create bufffer(shader)!");
        fclose(file);
        
        return 0;
    }
    
    if (size != fread(buffer, 1, size, file))
    {
        __LOG("Faild to read shader file!");
        free(buffer);
        fclose(file);
        
        return 0;
    }
    
    shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&buffer, NULL);
    glCompileShader(shader);
    
    free(buffer);
    fclose(file);
    
#ifdef DEBUG
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, (GLint*)&size);
    if (size > 0)
    {
        buffer = (GLchar*)malloc(size);
        glGetShaderInfoLog(shader, size, (GLint*)&size, buffer);
        __LOGARG("Shader compile log:\n%s!", buffer);
        
        free(buffer);
    }
#endif
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, (GLint*)&status);
    if (status == 0)
    {
        __LOGARG("Shader status is: %d!", status);
        glDeleteShader(shader);
        
        return status;
    }
    
    return shader;
}


int CShader::ValidateProgram(GLuint program)
{
    GLint logLength, status;
    
    glValidateProgram(program);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(program, logLength, &logLength, log);
        __LOGARG("Program validate log:\n%s", log);
        free(log);
    }
    
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (status == 0)
    {
        return 0;
    }
    
    return 1;
}

// CShaderFrame renders texture to current frame

CShaderFrame::CShaderFrame()
{
    
}

CShaderFrame::~CShaderFrame()
{
    
}

bool CShaderFrame::Init()
{
    m_gProgram = CShader::CreateProgram("textoframe.vsh", "textoframe.fsh", &m_gVertexShader, &m_gFragmentShader);
    
    if (m_gProgram != 0)
    {
        glBindAttribLocation(m_gProgram, SAL_POSITION, "position");
        glBindAttribLocation(m_gProgram, SAL_TEXTURE_COORD, "textureCoord");
        
        CShader::LinkProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);
        
        m_gUniformTexture = glGetUniformLocation(m_gProgram, "textureBuffer");
        
        return true;
    }
    
    return false;
}

void CShaderFrame::Deinit()
{
    CShader::DestroyProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);
}

GLint CShaderFrame::GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type)
{
    switch (Type)
    {
        case SUL_TEXTURE0:
            return m_gUniformTexture;
    }
    
    return -1;
}


CShaderDefault::CShaderDefault()
{
    
}

CShaderDefault::~CShaderDefault()
{
    
}

bool CShaderDefault::Init()
{
    m_gProgram = CShader::CreateProgram("default.vsh", "default.fsh", &m_gVertexShader, &m_gFragmentShader);
//    m_gProgram = CShader::CreateProgram("light.vsh", "light.fsh", &m_gVertexShader, &m_gFragmentShader);

    
    if (m_gProgram != 0)
    {
        glBindAttribLocation(m_gProgram, SAL_POSITION, "position");
        glBindAttribLocation(m_gProgram, SAL_NORMAL, "normal");

        CShader::LinkProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);
        
        m_gUniformModelView = glGetUniformLocation(m_gProgram, "modelview");
        m_gUniformProjection = glGetUniformLocation(m_gProgram, "projection");
        m_gUniformNormalMatrix = glGetUniformLocation(m_gProgram, "normalMatrix");
        m_gUniformLightPosition = glGetUniformLocation(m_gProgram, "lightPosition");
        
        return true;
    }
    
    return false;
}

void CShaderDefault::Deinit()
{
    CShader::DestroyProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);
}

GLint CShaderDefault::GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type)
{
    switch (Type)
    {
        case SUL_MODELVIEW:
            return m_gUniformModelView;
        
        case SUL_PROJECTION:
            return m_gUniformProjection;
        
        case SUL_NORMALMATRIX:
            return m_gUniformNormalMatrix;
            
        case SUL_LIGHTPOSITION:
            return m_gUniformLightPosition;
    }
    
    return -1;
}

CShaderTexture::CShaderTexture()
{
    
}

CShaderTexture::~CShaderTexture()
{
    
}

bool CShaderTexture::Init()
{
    m_gProgram = CShader::CreateProgram("texture.vsh", "texture.fsh", &m_gVertexShader, &m_gFragmentShader);
    
    if (m_gProgram != 0)
    {
        glBindAttribLocation(m_gProgram, SAL_POSITION, "position");
        glBindAttribLocation(m_gProgram, SAL_TEXTURE_COORD, "textureCoord");
        glBindAttribLocation(m_gProgram, SAL_NORMAL, "normal");
        
        CShader::LinkProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);
        
        m_gUniformModelView = glGetUniformLocation(m_gProgram, "modelview");
        m_gUniformProjection = glGetUniformLocation(m_gProgram, "projection");
        m_gUniformTexture = glGetUniformLocation(m_gProgram, "textureBuffer");
//        m_gUniformNormalMatrix = glGetUniformLocation(m_gProgram, "normalMatrix");
        
        return true;
    }
    
    return false;
}

void CShaderTexture::Deinit()
{
    CShader::DestroyProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);
}

GLint CShaderTexture::GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type)
{
    switch (Type)
    {
        case SUL_MODELVIEW:
            return m_gUniformModelView;
            
        case SUL_PROJECTION:
            return m_gUniformProjection;
            
        case SUL_TEXTURE0:
            return m_gUniformTexture;
    }
    
    return -1;
}

CShaderVerticalGlow::CShaderVerticalGlow()
{
    
}

CShaderVerticalGlow::~CShaderVerticalGlow()
{
    
}

bool CShaderVerticalGlow::Init()
{
    m_gProgram = CShader::CreateProgram("verticalBlur.vsh", "glow.fsh", &m_gVertexShader, &m_gFragmentShader);
    
    if (m_gProgram != 0)
    {
        glBindAttribLocation(m_gProgram, SAL_POSITION, "position");
        glBindAttribLocation(m_gProgram, SAL_TEXTURE_COORD, "textureCoord");
        
        CShader::LinkProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);
        
        m_gUniformTexture = glGetUniformLocation(m_gProgram, "textureBuffer");
        
        return true;
    }
    
    return false;
}

void CShaderVerticalGlow::Deinit()
{
    CShader::DestroyProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);
}

GLint CShaderVerticalGlow::GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type)
{
    if (Type == SUL_TEXTURE0)
        return m_gUniformTexture;
    
    return -1;
}

CShaderHorizontalGlow::CShaderHorizontalGlow()
{
    
}

CShaderHorizontalGlow::~CShaderHorizontalGlow()
{
    
}

bool CShaderHorizontalGlow::Init()
{
    m_gProgram = CShader::CreateProgram("horizontalBlur.vsh", "glow.fsh", &m_gVertexShader, &m_gFragmentShader);
    
    if (m_gProgram != 0)
    {
        glBindAttribLocation(m_gProgram, SAL_POSITION, "position");
        glBindAttribLocation(m_gProgram, SAL_TEXTURE_COORD, "textureCoord");
        
        CShader::LinkProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);
        
        m_gUniformTexture = glGetUniformLocation(m_gProgram, "textureBuffer");
        
        return true;
    }
    
    return false;
}

void CShaderHorizontalGlow::Deinit()
{
    CShader::DestroyProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);
}

GLint CShaderHorizontalGlow::GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type)
{
    if (Type == SUL_TEXTURE0)
        return m_gUniformTexture;
    
    return -1;
}

CShaderBlend::CShaderBlend()
{
    
}

CShaderBlend::~CShaderBlend()
{
    
}

bool CShaderBlend::Init()
{
    m_gProgram = CShader::CreateProgram("vertex.vsh", "blend.fsh", &m_gVertexShader, &m_gFragmentShader);
    
    if (m_gProgram != 0)
    {
        glBindAttribLocation(m_gProgram, SAL_POSITION, "position");
        glBindAttribLocation(m_gProgram, SAL_TEXTURE_COORD, "textureCoord");
        
        CShader::LinkProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);

        m_gUniformTexture0 = glGetUniformLocation(m_gProgram, "texture0");
        m_gUniformTexture1 = glGetUniformLocation(m_gProgram, "texture1");
        
        return true;
    }
    
    return false;
}

void CShaderBlend::Deinit()
{
    CShader::DestroyProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);
}

GLint CShaderBlend::GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type)
{
    switch (Type)
    {
        case SUL_TEXTURE0:
            return m_gUniformTexture0;
        case SUL_TEXTURE1:
            return m_gUniformTexture1;
    }
    
    return -1;
}

CShaderHeightFieldQuad::CShaderHeightFieldQuad()
{
    
}

CShaderHeightFieldQuad::~CShaderHeightFieldQuad()
{
    
}

bool CShaderHeightFieldQuad::Init()
{
    m_gProgram = CShader::CreateProgram("hf.vsh", "hf.fsh", &m_gVertexShader, &m_gFragmentShader);
    
    if (m_gProgram != 0)
    {
        glBindAttribLocation(m_gProgram, SAL_POSITION, "position");
        
        CShader::LinkProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);
        
        m_gUniformModelView = glGetUniformLocation(m_gProgram, "modelview");
        m_gUniformProjection = glGetUniformLocation(m_gProgram, "projection");
        m_gUniformNormalMatrix = glGetUniformLocation(m_gProgram, "normalMatrix");
        
        return true;
    }
    
    return false;
}

void CShaderHeightFieldQuad::Deinit()
{
    CShader::DestroyProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);
}

GLint CShaderHeightFieldQuad::GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type)
{
    switch (Type)
    {
        case SUL_MODELVIEW:
            return m_gUniformModelView;
        
        case SUL_PROJECTION:
            return m_gUniformProjection;
        
        case SUL_NORMALMATRIX:
            return m_gUniformNormalMatrix;
    }
    
    return -1;
}

// CShaderWave animates wave

CShaderWave::CShaderWave()
{
    
}

CShaderWave::~CShaderWave()
{
    
}

bool CShaderWave::Init()
{
    m_gProgram = CShader::CreateProgram("wave.vsh", "wave.fsh", &m_gVertexShader, &m_gFragmentShader);
    
    if (m_gProgram != 0)
    {
        glBindAttribLocation(m_gProgram, SAL_POSITION, "position");
        glBindAttribLocation(m_gProgram, SAL_TEXTURE_COORD, "textureCoord");
        
        CShader::LinkProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);
        
        m_gUniformScreen = glGetUniformLocation(m_gProgram, "screenTexture");
        m_gUniformParticle = glGetUniformLocation(m_gProgram, "particleTexture");
        
        return true;
    }
    
    return false;
}

void CShaderWave::Deinit()
{
    CShader::DestroyProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);
}

GLint CShaderWave::GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type)
{
    switch (Type)
    {
        case SUL_TEXTURE0:
            return m_gUniformScreen;
            
        case SUL_TEXTURE1:
            return m_gUniformParticle;

    }
    
    return -1;
}

// CShaderHeightField


CShaderHeightField::CShaderHeightField()
{
    
}

CShaderHeightField::~CShaderHeightField()
{
    
}

bool CShaderHeightField::Init()
{
    m_gProgram = CShader::CreateProgram("hf.vsh", "hf.fsh", &m_gVertexShader, &m_gFragmentShader);
    
    if (m_gProgram != 0)
    {
        glBindAttribLocation(m_gProgram, SAL_POSITION, "position");
        glBindAttribLocation(m_gProgram, SAL_NORMAL, "normal");
        
        CShader::LinkProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);
        
        m_gUniformModelView = glGetUniformLocation(m_gProgram, "modelview");
        m_gUniformProjection = glGetUniformLocation(m_gProgram, "projection");
        m_gUniformNormalMatrix = glGetUniformLocation(m_gProgram, "normalMatrix");
//        m_gUniformLightPosition = glGetUniformLocation(m_gProgram, "lightPosition");
        m_gUniformParticle = glGetUniformLocation(m_gProgram, "particleTexture");
        m_gUniformRadius = glGetUniformLocation(m_gProgram, "radius");
        
        return true;
    }
    
    return false;
}

void CShaderHeightField::Deinit()
{
    CShader::DestroyProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);
}

GLint CShaderHeightField::GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type)
{
    switch (Type)
    {
        case SUL_MODELVIEW:
            return m_gUniformModelView;
            
        case SUL_PROJECTION:
            return m_gUniformProjection;
            
        case SUL_NORMALMATRIX:
            return m_gUniformNormalMatrix;
            
        case SUL_LIGHTPOSITION:
            return m_gUniformLightPosition;
            
        case SUL_TEXTURE0:
            return m_gUniformParticle;
            
        case SUL_FLOAT0:
            return m_gUniformRadius;
    }
    
    return -1;
}

CShaderParticle::CShaderParticle()
{
    
}

CShaderParticle::~CShaderParticle()
{
    
}

bool CShaderParticle::Init()
{
    m_gProgram = CShader::CreateProgram("particle.vsh", "particle.fsh", &m_gVertexShader, &m_gFragmentShader);
    
    if (m_gProgram != 0)
    {
        glBindAttribLocation(m_gProgram, SAL_POSITION, "position");
        glBindAttribLocation(m_gProgram, SAL_TEXTURE_COORD, "textureCoord");
        
        CShader::LinkProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);
        
        m_gUniformProjection = glGetUniformLocation(m_gProgram, "projection");
        m_gUniformTexture = glGetUniformLocation(m_gProgram, "particle");
        
        return true;
    }
    
    return false;
}

void CShaderParticle::Deinit()
{
    CShader::DestroyProgram(m_gProgram, m_gVertexShader, m_gFragmentShader);
}

GLint CShaderParticle::GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type)
{
    switch (Type)
    {
        case SUL_PROJECTION:
            return m_gUniformProjection;
            
        case SUL_TEXTURE0:
            return m_gUniformTexture;
    }
    
    return -1;
}

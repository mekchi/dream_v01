//
//  IShader.h
//  dream
//
//  Created by Yermek Garifullanov on 10/21/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef __dream__IShader__
#define __dream__IShader__

#include "graphics.h"
#include "ShaderIds.h"
#include "VQMMath.h"

class CShader
{
    
public:
    
    CShader() {}
    virtual ~CShader() {}
    
    virtual bool Init() = 0;
    virtual void Deinit() = 0;
    virtual GLint GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type) = 0;
    
    GLuint GetProgram() { return m_gProgram; }

    static bool LoadShaders();
    static void DeloadShaders();
    static CShader* GetShader(E_SHADER_TYPE Type);
    static void SetProjection();
    
protected:
    
    static GLuint CreateProgram (const char *vtFileName, const char* fgFileName,
                          GLuint *vtShader, GLuint *fgShader);
    static void DestroyProgram(GLuint program, GLuint vtShader, GLuint fgShader);
    static int LinkProgram(GLuint program, GLuint vtShader, GLuint fgShader);
    static int ValidateProgram(GLuint program);
    
    GLuint m_gProgram;
    GLuint m_gVertexShader;
    GLuint m_gFragmentShader;
    
private:
    
    static GLuint CompileShader(const char *file, GLenum type);
    
    static GLuint m_arrayShaderProgram[NUM_SHADER_TYPE];
    static CShader* m_arrayShaderPointer[NUM_SHADER_TYPE];
    
};

class CShaderFrame : public CShader
{
public:
    
    CShaderFrame();
    virtual ~CShaderFrame();
    
    virtual bool Init();
    virtual void Deinit();
    virtual GLint GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type);
    
private:
    
    GLint m_gUniformTexture;
    
};

class CShaderDefault : public CShader
{
    
public:
    
    CShaderDefault();
    virtual ~CShaderDefault();
    
    virtual bool Init();
    virtual void Deinit();
    virtual GLint GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type);
    
private:
    
    GLint m_gUniformModelView;
    GLint m_gUniformProjection;
    GLint m_gUniformNormalMatrix;
    GLint m_gUniformLightPosition;
    
};

class CShaderTexture : public CShader
{
    
public:
    
    CShaderTexture();
    virtual ~CShaderTexture();
    
    virtual bool Init();
    virtual void Deinit();
    virtual GLint GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type);
    
private:
    
    GLint m_gUniformModelView;
    GLint m_gUniformProjection;
//    GLint m_gUniformNormalMatrix;
    GLint m_gUniformTexture;
    
};

class CShaderVerticalGlow : public CShader
{
    
public:
    
    CShaderVerticalGlow();
    virtual ~CShaderVerticalGlow();
    
    virtual bool Init();
    virtual void Deinit();
    virtual GLint GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type);
    
private:
    
    GLint m_gUniformTexture;
    
};

class CShaderHorizontalGlow : public CShader
{
    
public:
    
    CShaderHorizontalGlow();
    virtual ~CShaderHorizontalGlow();
    
    virtual bool Init();
    virtual void Deinit();
    virtual GLint GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type);
    
private:
    
    GLint m_gUniformTexture;
    
};

class CShaderBlend : public CShader
{
    
public:
    
    CShaderBlend();
    virtual ~CShaderBlend();
    
    virtual bool Init();
    virtual void Deinit();
    virtual GLint GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type);
    
private:
    
    GLint m_gUniformTexture0;
    GLint m_gUniformTexture1;
    
};

class CShaderWave : public CShader
{
public:
    
    CShaderWave();
    virtual ~CShaderWave();
    
    virtual bool Init();
    virtual void Deinit();
    virtual GLint GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type);
    
private:
    
    GLint m_gUniformScreen;
    GLint m_gUniformParticle;
    GLint m_gUniformParams;
};

class CShaderHeightField : public CShader
{
    
public:
    
    CShaderHeightField();
    virtual ~CShaderHeightField();
    
    virtual bool Init();
    virtual void Deinit();
    virtual GLint GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type);
    
private:
    
    GLint m_gUniformModelView;
    GLint m_gUniformProjection;
    GLint m_gUniformNormalMatrix;
    GLint m_gUniformLightPosition;
    GLint m_gUniformParticle;
    GLint m_gUniformRadius;
    
};

class CShaderParticle : public CShader
{
    
public:
    
    CShaderParticle();
    virtual ~CShaderParticle();
    
    virtual bool Init();
    virtual void Deinit();
    virtual GLint GetUnivormLocation(E_SHADER_UNIFORM_LOCATION Type);
    
private:
    
    GLint m_gUniformProjection;
    GLint m_gUniformTexture[2];
    
};

#endif /* defined(__dream__IShader__) */

////
////  IMaterial.h
////  dream
////
////  Created by Yermek Garifullanov on 10/8/13.
////  Copyright (c) 2013 mekchi. All rights reserved.
////
//
//#ifndef __dream__IMaterial__
//#define __dream__IMaterial__
//
//#include "Ids.h"
//#include "DataTypes.h"
//#include "Graphics.h"
//
//class IMaterial
//{
//
//public:
//    
//    IMaterial(IDrawObject* DrawObject) : m_opDrawObject(DrawObject) {}
//    virtual ~IMaterial() {}
//    
//    virtual bool Init(SMaterialData* Data = nullptr) = 0;
//    virtual void Deinit() = 0;
//    virtual void Render(SMaterialRenderData* Data = nullptr) = 0;
//    
//    static bool FrameBufferInit();
//    static void FrameBufferDeinit();
//    static void RenderFrameBuffer();
//    
//protected:
//    
//    static void RenderQuad();
//    static GLint GetScreenTexture();
//    
//    IDrawObject* m_opDrawObject;
//
//    static GLfloat GetViewWidth();
//    static GLfloat GetViewHeight();
//
//private:
//
//    // temp, suposed to render to texture
//    // dont know why doesnt render to texutre
//    // texture is black
//    // now we have to copy to texture every time
//    
//    static GLuint m_gFB;
//    static GLuint m_gRB;
//    static GLuint m_gRBDepth;
//    static GLuint m_gMainTexFrameBuffer;
//    static GLuint m_gVBO;
//    static GLuint m_gVAO;
//    
//    static GLfloat m_fViewportWidth;
//    static GLfloat m_fViewportHeight;
//    
//};
//
//class CMaterialDefault : public IMaterial
//{
//    
//public:
//    
//    CMaterialDefault(IDrawObject* DrawObject);
//    virtual ~CMaterialDefault();
//    
//    virtual bool Init(SMaterialData* Data = nullptr);
//    virtual void Deinit();
//    virtual void Render(SMaterialRenderData* Data = nullptr);
//    
//private:
//    
//    GLuint m_gTexture;
//    
//};
//
//class CMaterialTexture : public IMaterial
//{
//    
//public:
//    
//    CMaterialTexture(IDrawObject* DrawObject);
//    virtual ~CMaterialTexture();
//    
//    virtual bool Init(SMaterialData* Data = nullptr);
//    virtual void Deinit();
//    virtual void Render(SMaterialRenderData* Data = nullptr);
//    
//private:
//    
//    GLuint m_gTexture;
//    
//};
//
//class CMaterialGlow : public IMaterial
//{
//    
//public:
//    
//    CMaterialGlow(IDrawObject* DrawObject);
//    virtual ~CMaterialGlow();
//    
//    virtual bool Init(SMaterialData* Data = nullptr);
//    virtual void Deinit();
//    virtual void Render(SMaterialRenderData* Data = nullptr);
//    
//private:
//    
//    GLuint m_gFB;
//    GLuint m_gRB;
//    GLuint m_gTexture[2];
//    
//    GLuint m_gVAO;
//    GLuint m_gVBO;
//    
//    GLfloat m_gTextureWidth;
//    GLfloat m_gTextureHeight;
//    
//};
//
//class CMaterialWave : public IMaterial
//{
//    
//public:
//    
//    CMaterialWave();
//    virtual ~CMaterialWave();
//    
//    virtual bool Init(SMaterialData* Data = nullptr);
//    virtual void Deinit();
//    virtual void Render(SMaterialRenderData* Data = nullptr);
//    
//private:
//
//     GLuint m_gTexture;
//    
//};
//
//class CMaterialHeightField : public IMaterial
//{
//    
//public:
//    
//    CMaterialHeightField(IDrawObject* DrawObject);
//    virtual ~CMaterialHeightField();
//    
//    virtual bool Init(SMaterialData* Data = nullptr);
//    virtual void Deinit();
//    virtual void Render(SMaterialRenderData* Data = nullptr);
//    
//private:
//    
//    GLuint m_gTexture;
//    
//};
//
//#endif /* defined(__dream__IMaterial__) */

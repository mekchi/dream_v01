
/* mekchi */

#include "LoadTools.h"

#include <stdlib.h>
#include <stdio.h>

#include "Graphics.h"
#include "Log.h"
#include "Globals.h"

bool LoadFileBytes(const char *filename, unsigned char** data, int* size)
{
    FILE* file = nullptr;
    
    std::string path = Globals::GetResourcePath() + "/" + filename;
    
    if ((file = fopen(path.c_str(), "rb")) == NULL)
    {
        __LOGARG("Faild to open %s", filename);
        
        return false;
    }
    
    (*size) = 0;
    fseek(file, 0, SEEK_END);
    (*size) = (int)ftell(file);
    fseek(file, 0, SEEK_SET);
    
    (*data) = nullptr;
    (*data) = new unsigned char[*size];
    
    if ((*data) == nullptr)
    {
        fclose(file);
        __LOG("Faild to allocate memory");
        
        return false;
    }
    
    if (fread((*data), 1, (*size), file) != (*size))
    {
        fclose(file);
        delete (*data);
        __LOGARG("Faild to read %s", filename);
        
        return false;
    }
    
    return true;
}


static unsigned int getint(FILE *fp);
static unsigned int getshort(FILE *fp);

int loadBmpImage(const char *filename)
{
    FILE *file;
    unsigned long size;                 /*  size of the image in bytes. */
    unsigned long i;                    /*  standard counter. */
    unsigned short int planes;          /*  number of planes in image (must be 1)  */
    unsigned short int bpp;             /*  number of bits per pixel (must be 24) */
    char temp;                          /*  used to convert bgr to rgb color. */
    
    int width, height;
    unsigned char *data = NULL;
    //    GLuint texture = 0;
    
    std::string path = Globals::GetResourcePath() + "/" + filename;
    
    
    /*  make sure the file is there. */
    if ((file = fopen(path.c_str(), "rb")) == NULL)
    {
        __LOGARG("File Not Found : %s", filename);
        
        return 0;
    }
    
    //    fseek(file, 0, SEEK_END);
    //    size = ftell(file);
    //    fseek(file, 0, SEEK_SET);
    //
    //    data = (unsigned char*)malloc(size);
    //        fread(data, 1, size, file);
    //
    //    FILE *tempFile = fopen("/Users/mekchi/Desktop/temp.bmp", "wb");
    //
    //    fwrite(data, 1, size, tempFile);
    //
    //    fclose(tempFile);
    //    free(data);
    
    
    
    /*  seek through the bmp header, up to the width height: */
    fseek(file, 18, SEEK_CUR);
    
    /*  No 100% errorchecking anymore!!! */
    
    /*  read the width */
	width = getint (file);
    
    /*  read the height */
    height = getint (file);
    
    /*  calculate the size (assuming 24 bits or 3 bytes per pixel). */
    size = width * height * 3;
    
    /*  read the planes */
    planes = getshort(file);
    if (planes != 1)
    {
        __LOGARG("Planes from %s is not 1: %u", filename, planes);
        fclose(file);
        
        return 0;
    }
    
    /*  read the bpp */
    bpp = getshort(file);
    if (bpp != 24)
    {
        __LOGARG("Bpp from %s is not 24: %u", filename, bpp);
        fclose(file);
        
        return 0;
    }
	
    /*  seek past the rest of the bitmap header. */
    fseek(file, 24, SEEK_CUR);
    
    /*  read the data.  */
    data = (unsigned char *)malloc(size);
    if (data == NULL)
    {
        __LOG("Error allocating memory for color-corrected image data");
        fclose(file);
        
        return 0;
    }
    
    if ((i = fread(data, 1, size, file)) != size)
    {
        __LOGARG("Error reading image data from %s", filename);
        fclose(file);
        free(data);
        
        return 0;
    }
    
    for (i = 0; i < size; i += 3)
    { /*  reverse all of the colors. (bgr -> rgb) */
        temp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = temp;
    }
    
    fclose(file);
    
    
    
    //glBindTexture(GL_TEXTURE_2D, texture);
    
    
    
    //	glBlendFunc(GL_ONE, GL_SRC_COLOR);
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
    //                 width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    
    //        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    
    
    
    free(data);
    
    return 1;
}

static unsigned int getint(FILE *fp)
{
	int c, c1, c2, c3;
    
	/*  get 4 bytes */
	c = getc(fp);
	c1 = getc(fp);
	c2 = getc(fp);
	c3 = getc(fp);
    
	return ((unsigned int) c) +
    (((unsigned int) c1) << 8) +
    (((unsigned int) c2) << 16) +
    (((unsigned int) c3) << 24);
}

static unsigned int getshort(FILE *fp)
{
	int c, c1;
    
	/* get 2 bytes*/
	c = getc(fp);
	c1 = getc(fp);
    
	return (unsigned int)c + (((unsigned int)c1) << 8);
}
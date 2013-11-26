
/* mekchi */

#ifndef _MEKCHI_LT_
#define _MEKCHI_LT_

int loadBmpImage(const char *filename);



bool LoadFileBytes(const char *filename, unsigned char** data, int* size);

bool LoadPngBytes(const char *filename, unsigned char** data, unsigned int* width, unsigned int* height);
void DeletePngBytes(unsigned char* data);

#endif

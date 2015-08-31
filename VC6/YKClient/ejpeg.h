// jpeg encoder v1.0
// Bugs report: michael_huyl@21cn.com

typedef unsigned char   BYTE;
typedef unsigned short  uint16;
typedef unsigned long   DWORD;

void RGBtoJPEGBuff(BYTE* Buff,DWORD ImageWidth,DWORD ImageHeight,BYTE *outJPEGBuff,DWORD *BuffLen);
void RGBtoJPEGFile(BYTE* Buff,DWORD ImageWidth,DWORD ImageHeight,char* outFileName);

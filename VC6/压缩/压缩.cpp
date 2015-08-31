// 压缩.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "My minilzo.h"
#include "resource.h"
#include "windows.h"
#include "Commdlg.h"

typedef struct
{
	unsigned long org_data_size;//未压缩的数据大小
	unsigned long cmp_data_size;//压缩后的数据大小
}COMP_DATA, *P_COMP_DATA;

#define DATA(x) x+8

BOOL OpenFileDlg(HWND hWnd,char *szFilter,char *pszFile)
{
	OPENFILENAME of;

	ZeroMemory(&of,sizeof(of));
	ZeroMemory(pszFile,MAX_PATH);
	of.lStructSize=sizeof(of);
	of.hwndOwner=hWnd;
	of.lpstrFilter=szFilter;
	of.lpstrFile=pszFile;
	of.nMaxFile=MAX_PATH;
	of.Flags=OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
	return GetOpenFileName(&of);
}

void GetFileTypeSz(char *pszFile,char *pszExp)
{
	char *p;
	p=pszFile;
	while((*p)!='.')
	{
		p++;
		if(*p==0)
			return;
	}
	p++;
	lstrcpy(pszExp,p);
	GetFileTypeSz(p,pszExp);//递归调用，找出最后一个扩展名
}

void strcut(char *szStr,int n)
{
	szStr[n]=0;
	szStr[n+1]=0;
	szStr[n+2]=0;
}

int CALLBACK ProcDlgMain (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char str[MAX_PATH];
	char szPath[MAX_PATH];
	unsigned long size,nSfile,nDfile;
	unsigned char *pSData,*pDData;
	DWORD dwRead,dwWrite;
	HANDLE hFileS,hFileD;
	P_COMP_DATA pcd;

	switch(message)
	{
	case WM_COMMAND:
		  switch(LOWORD(wParam))
		  {
		  case IDC_SELFILE:
			  if(OpenFileDlg(hwnd,"任意文件\0*.*\0",szPath))
			  {
				  SetDlgItemText(hwnd,IDC_FILEPATH,szPath);
				  GetFileTypeSz(szPath,str);
				  if(!lstrcmp(str,"lzl"))//如果扩展名是lzl，则为压缩文件
				  {
					  EnableWindow(GetDlgItem(hwnd,IDC_DECOMPRESS),1);
					  EnableWindow(GetDlgItem(hwnd,IDC_COMPRESS),0);
					  break;
				  }
				  EnableWindow(GetDlgItem(hwnd,IDC_COMPRESS),1);
				  EnableWindow(GetDlgItem(hwnd,IDC_DECOMPRESS),0);
			  }
			  break;
		  case IDC_COMPRESS:
			  GetDlgItemText(hwnd,IDC_FILEPATH,szPath,MAX_PATH);
			  //打开源文件
			  hFileS=CreateFile(szPath,GENERIC_ALL,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			  if(hFileS==INVALID_HANDLE_VALUE)
			  {
				  MessageBox(hwnd,"文件打开失败","提示",MB_OK);
				  return 1;
			  }
			  lstrcpy(str,szPath);
			  lstrcat(str,".lzl");			  
			  //打开目标文件
			  hFileD=CreateFile(str,GENERIC_ALL,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			  if(hFileD==INVALID_HANDLE_VALUE)
			  {
				  MessageBox(hwnd,"文件创建失败","提示",MB_OK);
				  return 1;
			  }
			  //申请内存空间
			  nSfile=GetFileSize(hFileS,NULL);
			  pSData=(BYTE *)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT,nSfile);
			  if(!pSData)
			  {
				  MessageBox(hwnd,"申请源文件内存空间失败","提示",MB_OK);
				  break;
			  }
			  if(!ReadFile(hFileS,pSData,nSfile,&dwRead,0))
			  {
				  MessageBox(hwnd,"读取源文件失败","提示",MB_OK);
				  break;
			  }
			  pDData=(BYTE *)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT,nSfile+nSfile/16+64+3);
			  if(!pDData)
			  {
				  MessageBox(hwnd,"申请目标文件内存空间失败","提示",MB_OK);
				  break;
			  }
			  pcd=(P_COMP_DATA)pDData;
			  pcd->org_data_size=nSfile;//填写源文件大小
			  lzo_init();//初始化minilzo库
			  if(Compress(pSData,nSfile,DATA(pDData),&size,wrkmem)!=LZO_E_OK)
			  {
				  MessageBox(hwnd,"压缩失败","提示",MB_OK);
				  break;
			  }
			  pcd->cmp_data_size=size;//填写压缩后数据大小
			  if(!WriteFile(hFileD,pDData,size,&dwWrite,0))
			  {
				  MessageBox(hwnd,"写入目标文件失败","提示",MB_OK);
				  break;
			  }
			  CloseHandle(hFileS);CloseHandle(hFileD);
			  MessageBox(hwnd,"压缩成功","提示",MB_OK);
			  break;
		  case IDC_DECOMPRESS:
			  GetDlgItemText(hwnd,IDC_FILEPATH,szPath,MAX_PATH);
			  //打开源文件
			  hFileS=CreateFile(szPath,GENERIC_ALL,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			  if(hFileS==INVALID_HANDLE_VALUE)
			  {
				  MessageBox(hwnd,"文件打开失败","提示",MB_OK);
				  return 1;
			  }
			  lstrcpy(str,szPath);
			  strcut(str,lstrlen(str)-4);
			  //打开目标文件
			  hFileD=CreateFile(str,GENERIC_ALL,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			  if(hFileD==INVALID_HANDLE_VALUE)
			  {
				  MessageBox(hwnd,"文件创建失败","提示",MB_OK);
				  return 1;
			  }
			  //申请内存空间
			  nSfile=GetFileSize(hFileS,NULL);
			  pSData=(BYTE *)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT,nSfile);
			  if(!pSData)
			  {
				  MessageBox(hwnd,"申请源文件内存空间失败","提示",MB_OK);
				  break;
			  }
			  ReadFile(hFileS,pSData,nSfile,&dwRead,0);
			  pcd=(P_COMP_DATA)pSData;
			  pDData=(BYTE *)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT,pcd->org_data_size);
			  if(!pDData)
			  {
				  MessageBox(hwnd,"申请目标文件内存空间失败","提示",MB_OK);
				  break;
			  }
			  lzo_init();//初始化minilzo库
			  size=pcd->org_data_size;
			  if(lzo1x_decompress_safe(DATA(pSData),pcd->cmp_data_size,pDData,&size,NULL)!=LZO_E_OK)
			  {
				  //MessageBox(hwnd,"解压缩失败","提示",MB_OK);
				  //break;
			  }
			  WriteFile(hFileD,pDData,size,&dwWrite,0);
			  CloseHandle(hFileS);CloseHandle(hFileD);
			  MessageBox(hwnd,"解压缩成功","提示",MB_OK);
			  break;
		  }
		  break;
	case WM_CLOSE:
		EndDialog(hwnd,0);
		break;
	default:
		return 0;
	}
	return 1;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	DialogBoxParam(hInstance,(const char *)IDD_DLGMAIN,NULL,ProcDlgMain,0);
	return 0;
}




// 压缩.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "windows.h"
#include "Commdlg.h"

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

BOOL AddTextToEdit(HWND hEdit,char *szTxt,BOOL bNextLine)
{
	static char NextLine[]={13,10,0};

	SendMessage(hEdit,EM_SETSEL,-2,-1);
	SendMessage(hEdit,EM_REPLACESEL,0,(long)szTxt);
	if(bNextLine)
		SendMessage(hEdit,EM_REPLACESEL,0,(long)NextLine);
	return 1;
}

int CALLBACK ProcDlgMain (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char str[MAX_PATH];
	char szPath[MAX_PATH];
	int min,i;
	int df,nfile1,nfile2;
	static HWND hOut;
	BYTE *pData1,*pData2;
	DWORD dwRead,dwWrite;
	HANDLE hFile1,hFile2;

	switch(message)
	{
	case WM_INITDIALOG:
		 hOut=GetDlgItem(hwnd,IDC_OUTINFO);
		 break;
	case WM_COMMAND:
		  switch(LOWORD(wParam))
		  {
		  case IDC_SEL1:
			  OpenFileDlg(hwnd,"任意文件\0*.*\0",str);
			  SetDlgItemText(hwnd,IDC_FILE1,str);
			  break;
		  case IDC_SEL2:
			  OpenFileDlg(hwnd,"任意文件\0*.*\0",str);
			  SetDlgItemText(hwnd,IDC_FILE2,str);
			  break;
		  case IDOK:
			  GetDlgItemText(hwnd,IDC_FILE1,szPath,MAX_PATH);
			  //打开文件1
			  hFile1=CreateFile(szPath,GENERIC_ALL,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			  if(hFile1==INVALID_HANDLE_VALUE)
			  {
				  MessageBox(hwnd,"文件1打开失败","提示",MB_OK);
				  return 1;
			  }
			  GetDlgItemText(hwnd,IDC_FILE2,str,MAX_PATH);
			  //打开文件2
			  hFile2=CreateFile(str,GENERIC_ALL,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			  if(hFile2==INVALID_HANDLE_VALUE)
			  {
				  MessageBox(hwnd,"文件2打开失败","提示",MB_OK);
				  return 1;
			  }
			  //申请内存空间
			  nfile1=GetFileSize(hFile1,NULL);
			  pData1=(BYTE *)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT,nfile1);
			  if(!pData1)
			  {
				  MessageBox(hwnd,"申请文件1内存空间失败","提示",MB_OK);
				  break;
			  }
			  ReadFile(hFile1,pData1,nfile1,&dwRead,0);
			  nfile2=GetFileSize(hFile2,NULL);
			  pData2=(BYTE *)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT,nfile2);
			  if(!pData2)
			  {
				  MessageBox(hwnd,"申请文件2内存空间失败","提示",MB_OK);
				  break;
			  }
			  ReadFile(hFile2,pData2,nfile2,&dwRead,0);
			  wsprintf(str,"文件1大小：%u，文件2大小：%d",nfile1,nfile2);
			  AddTextToEdit(hOut,str,1);
			  min=nfile1<nfile2?nfile1:nfile2;
			  df=0;
			  for(i=0;i<min;i++)
			  {
				  if(pData1[i]!=pData2[i])
				  {
					  wsprintf(str,"%d:\t%d\t%d\t%c\t%c\t",i,pData1[i],pData2[i]),pData1[i],pData2[i];
					  AddTextToEdit(hOut,str,1);
					  df++;
				  }
			  }
			  wsprintf(str,"共%d处不同",df);
			  MessageBox(hwnd,str,"比较完成！",MB_OK);
			  CloseHandle(hFile1);CloseHandle(hFile2);
			  break;
		  case IDCANCEL:
			  EndDialog(hwnd,0);
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



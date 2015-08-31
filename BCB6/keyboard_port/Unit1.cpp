//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TKeyboard *Keyboard;
//---------------------------------------------------------------------------
__fastcall TKeyboard::TKeyboard(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
HANDLE hDevice;
unsigned char Key_code='a';
class key
{
  public:
    unsigned char scan;
    void key_down(char code)
    {
      char ch=code;
      ch&=0x7f;
      FileWrite((int)hDevice,(const void *)&ch,1);
    }
    void key_up(char code)
    {
      char ch=code;
      ch&=0x80;
      FileWrite((int)hDevice,(const void *)&ch,1);
    }
}Keys;
//安装驱动  参数1：服务名称   参数二：驱动文件路径
bool InstallService(char *szDriver,char *szFile,bool bStart)
{
   SC_HANDLE sh=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
   if(!sh)return false;
   SC_HANDLE rh=CreateService(sh,szDriver,szDriver,SERVICE_ALL_ACCESS,
                                                   SERVICE_KERNEL_DRIVER,
                                                   SERVICE_DEMAND_START,
                                                   SERVICE_ERROR_NORMAL,
                                                   szFile,NULL,NULL,NULL,
                                                   NULL,NULL);
   if(!rh)
   {
     if(GetLastError()==ERROR_SERVICE_EXISTS)
     {
        //服务已安装的情况
        rh=OpenService(sh,szDriver,SERVICE_ALL_ACCESS);
        if(!rh)
        {
          CloseServiceHandle(sh);
          return false;
        }
     }
     else
     {
        CloseServiceHandle(sh);
        return false;
     }
   }
   if(bStart)                //是否要启动服务
     StartService(rh,0,NULL);
   CloseServiceHandle(sh);
   CloseServiceHandle(rh);
   return true;
}

void __fastcall TKeyboard::FormCreate(TObject *Sender)
{
   char str[260];
   char szSys[260];
   //完成一些初始化工作
   //-------------------------
   //安装驱动
   GetCurrentDirectory(260,str);
   wsprintf(szSys,"%s\\Keyboard.sys",str);
   if(!InstallService("mykey",szSys,1)
   {
     ShowMessage("驱动加载失败");
     ExitProcess(0);
   }
   //全局设备（文件）句柄   初始化
   //hDevice=(HANDLE)FileOpen(L"\\.\\\\wriKeyPort",fmOpenReadWrite);
   hDevice=CreateFileW(L"\\\\.\\wriKeyPort",GENERIC_READ|GENERIC_WRITE,0,
                        NULL,OPEN_EXISTING,0,NULL);
   if(hDevice==(HANDLE)INVALID_HANDLE_VALUE)
   {
     wsprintf(str,"设备打开失败，错误代码：%d",GetLastError());
     ShowMessage(str);
     ExitProcess(0);
   }
}
//---------------------------------------------------------------------------

void __fastcall TKeyboard::StartClick(TObject *Sender)
{
   Timer1->Interval=Interval->Text.ToInt();
   Timer1->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TKeyboard::Timer1Timer(TObject *Sender)
{
  if(down->Checked)
    Keys.key_down(Key_code);
  if(up->Checked)
    Keys.key_up(Key_code);
}
//---------------------------------------------------------------------------
void __fastcall TKeyboard::EndClick(TObject *Sender)
{
  Timer1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TKeyboard::KeyCodeKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   char str[20];
   Key_code=MapVirtualKey(Key,0);
   wsprintf(str,"扫描码：0x%x",Key);
   KeyCode->Text=str;
}
//---------------------------------------------------------------------------


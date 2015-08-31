//---------------------------------------------------------------------------

#include <vcl.h>
#include <Registry.hpp>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TWork *Work;
//---------------------------------------------------------------------------
__fastcall TWork::TWork(TComponent* Owner)
        : TForm(Owner)
{
DragAcceptFiles(this->Handle,true);
}
//---------------------------------------------------------------------------
void GetFileName(char *szPath,char *pszName) //得到不含路径的文件名
{
        int i,j;
        for(i=0;szPath[i]!=0;i++)
                if(szPath[i]=='\\')j=i;
        strcpy(pszName,&szPath[++j]);
}
TRegistry *Reg=new TRegistry;
void UpadateList(TObject *Sender)      //刷新工作列表
{
        char szName[128]={0};
        unsigned long b,i=0,nName;
        Work->NameList->Clear();
        //枚举键值
        while(1)
        {
        nName=128;
        b=RegEnumValue(Reg->CurrentKey,i,szName,&nName,0,NULL,NULL,NULL);
        if(b==ERROR_NO_MORE_ITEMS)break;
        Work->NameList->AddItem(szName,Sender);
        i++;
        }
}

void __fastcall AddListView(char *szPath)  //向列表视图控件添加一个路径
{
        char szName[256]={0};
        GetFileName(szPath,szName);
        TListItem *item;
        item=Work->ListView1->Items->Add();
        item->Caption=szName;
        item->ImageIndex=0;
        item->SubItems->Add(szPath);
}

void UpadatePathList(char *szData)     //刷新路径  视图列表
{
   char str[256]={0};
   char *p=str;
   int i;
   Work->ListView1->Clear();
   for(i=0;szData[i]!=0;i++,p++)
   {
     (*p)=szData[i];
     if((*p)==';')
     {
       (*p)=0;
       AddListView(str);
       p=str-1;
     }
   }
}

void __fastcall TWork::AddClick(TObject *Sender)  //添加新工作
{
int ItemCount,i;
if(Name->GetTextLen()!=0)
{
        char szName[128];
        //检查同名项
        ItemCount=NameList->Count;--ItemCount;
        for(i=0;i<=ItemCount;i++)
        if(Name->Text==NameList->Items->Strings[i])
        {
                ShowMessage("存在同名项");
                return ;
        }
        //添加到注册表
        strcpy(szName,Name->Text.c_str());
        Reg->WriteString(szName,NULL);
        UpadateList(Sender);
}
}
//---------------------------------------------------------------------------


void __fastcall TWork::NameKeyPress(TObject *Sender, char &Key)
{
if(Key==13)      //Enter键按下
{
AddClick(Sender);
Name->Clear();
}
}
//---------------------------------------------------------------------------
void __fastcall SavaData()        //保存到注册表
{
   int i,j=Work->ListView1->Items->Count; j--;
   char szData[10240],*pszPath; szData[0]=0;
   for(i=0;i<=j;i++)
   {
      pszPath=Work->ListView1->Items->Item[i]->SubItems->Strings[0].c_str();
      strcat(szData,pszPath);
      strcat(szData,";");
   }
   Reg->WriteString(Work->NameList->Items->Strings[Work->NameList->ItemIndex],
                        szData);
}

void __fastcall TWork::AcceptFiles(TMessage& Msg)
{
    int i,m_nCount;
    char szPath[256];
    if(NameList->ItemIndex==-1)
    {
        ShowMessage("请先选择一项");
        return ;
    }
    m_nCount=DragQueryFile((HANDLE)Msg.WParam,0xffffffff,szPath,256);
    for(i=0;i<m_nCount;i++)
    {
        DragQueryFile((HANDLE)Msg.WParam,i,szPath,256);
        AddListView(szPath);
        SavaData();
    }
    DragFinish((HANDLE)Msg.WParam);
}
void __fastcall TWork::FormCreate(TObject *Sender)
{
        Reg->RootKey=HKEY_LOCAL_MACHINE;
        if(!Reg->OpenKey("SoftWare\\Work",1))
           ShowMessage("注册表打开失败");
        UpadateList(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TWork::NameListClick(TObject *Sender)
{
  AnsiString strName;
  int index=NameList->ItemIndex;
  if(index!=-1) //如果选中了一项
  {
    strName=Reg->ReadString(NameList->Items->Strings[index]);
    UpadatePathList(strName.c_str());
  }
}
//--------------------------------------------------------------------------

void __fastcall TWork::NameListKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
     int index=NameList->ItemIndex;
     if(index!=-1)
      switch(Key)
      {
      case VK_DELETE:
        Reg->DeleteValue(NameList->Items->Strings[index]);
        NameList->DeleteSelected();
        break;
      case VK_RETURN:
        NameListDblClick(Sender);
        break;
      }
}
//---------------------------------------------------------------------------


void __fastcall TWork::NameListDblClick(TObject *Sender)
{
  int i,j=Work->ListView1->Items->Count; j--;
   char *pszPath;
   for(i=0;i<=j;i++)
   {
      pszPath=Work->ListView1->Items->Item[i]->SubItems->Strings[0].c_str();
      ShellExecute(NULL,"open",pszPath,NULL,NULL,SW_SHOWNORMAL);
   }
}
//---------------------------------------------------------------------------
void __fastcall TWork::ListView1DblClick(TObject *Sender)
{
  char *pszPath;
  pszPath=ListView1->Selected->SubItems->Strings[0].c_str();
  ShellExecute(NULL,"open",pszPath,NULL,NULL,SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TWork::ListView1KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
      switch(Key)
      {
      case VK_DELETE:
        ListView1->DeleteSelected();
        SavaData();
        break;
      case VK_RETURN:
        ListView1DblClick(Sender);
        break;
      }
}
//---------------------------------------------------------------------------





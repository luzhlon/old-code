//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TWork : public TForm
{
__published:	// IDE-managed Components
        TEdit *Name;
        TButton *Add;
        TListBox *NameList;
        TListView *ListView1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        void __fastcall AddClick(TObject *Sender);
        void __fastcall NameKeyPress(TObject *Sender, char &Key);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall NameListClick(TObject *Sender);
        void __fastcall NameListKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall NameListDblClick(TObject *Sender);
        void __fastcall ListView1KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall ListView1DblClick(TObject *Sender);
private:	// User declarations
        void __fastcall AcceptFiles(TMessage& Msg);
public:		// User declarations
        __fastcall TWork(TComponent* Owner);
        BEGIN_MESSAGE_MAP
        VCL_MESSAGE_HANDLER(WM_DROPFILES,TMessage,AcceptFiles)
        END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TWork *Work;
//---------------------------------------------------------------------------
#endif

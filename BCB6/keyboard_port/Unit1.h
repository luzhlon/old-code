//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TKeyboard : public TForm
{
__published:	// IDE-managed Components
        TEdit *KeyCode;
        TLabel *Label1;
        TEdit *Interval;
        TLabel *Label2;
        TButton *Start;
        TTimer *Timer1;
        TCheckBox *down;
        TCheckBox *up;
        TButton *End;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall StartClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall EndClick(TObject *Sender);
        void __fastcall KeyCodeKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
        __fastcall TKeyboard(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TKeyboard *Keyboard;
//---------------------------------------------------------------------------
#endif

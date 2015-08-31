object Form1: TForm1
  Left = 454
  Top = 177
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Work Environment'
  ClientHeight = 374
  ClientWidth = 668
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = #26032#23435#20307
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 16
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 70
    Height = 20
    Caption = #24037#20316#29615#22659#65306
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = #24494#36719#38597#40657
    Font.Style = []
    ParentFont = False
  end
  object NameList: TListBox
    Left = 8
    Top = 64
    Width = 185
    Height = 297
    ItemHeight = 16
    TabOrder = 0
  end
  object ListView1: TListView
    Left = 200
    Top = 32
    Width = 457
    Height = 329
    Columns = <
      item
        Caption = #25991#20214#21517
        Width = 200
      end
      item
        Caption = #36335#24452
        Width = 600
      end>
    TabOrder = 1
    ViewStyle = vsReport
  end
  object Name: TEdit
    Left = 8
    Top = 32
    Width = 137
    Height = 24
    TabOrder = 2
  end
  object Add: TButton
    Left = 152
    Top = 32
    Width = 41
    Height = 25
    Caption = '+'
    TabOrder = 3
    OnClick = AddClick
  end
end

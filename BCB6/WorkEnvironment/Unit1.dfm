object Work: TWork
  Left = 294
  Top = 173
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'WorkEnvironment'
  ClientHeight = 540
  ClientWidth = 739
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = #23435#20307
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 14
  object Label1: TLabel
    Left = 8
    Top = 496
    Width = 329
    Height = 14
    Caption = '*'#26412#36719#20214#20026#19981#21516#30340#24037#20316#12289#23398#20064#29615#22659#25209#37327#25171#24320#25351#23450#30340#25991#20214
  end
  object Label2: TLabel
    Left = 8
    Top = 518
    Width = 252
    Height = 14
    Caption = '*'#21019#24314#19968#20010#24037#20316#29615#22659#21518#25353'+'#38190#21152#20837#24037#20316#21015#34920
  end
  object Label3: TLabel
    Left = 368
    Top = 496
    Width = 329
    Height = 14
    Alignment = taCenter
    Caption = '*'#36873#20013#20219#24847#19968#39033#21518#21452#20987#25110'Enter'#38190#20026#25171#24320#65292'Del'#38190#20026#21024#38500
  end
  object Label4: TLabel
    Left = 368
    Top = 517
    Width = 329
    Height = 14
    Caption = '*********'#20316#32773#65306'CodeSoul     2013.10.4**********'
  end
  object Name: TEdit
    Left = 8
    Top = 16
    Width = 129
    Height = 22
    TabOrder = 0
    OnKeyPress = NameKeyPress
  end
  object Add: TButton
    Left = 144
    Top = 16
    Width = 33
    Height = 22
    Caption = '+'
    TabOrder = 1
    OnClick = AddClick
  end
  object NameList: TListBox
    Left = 8
    Top = 40
    Width = 169
    Height = 449
    ItemHeight = 14
    TabOrder = 2
    OnClick = NameListClick
    OnDblClick = NameListDblClick
    OnKeyDown = NameListKeyDown
  end
  object ListView1: TListView
    Left = 184
    Top = 16
    Width = 548
    Height = 473
    Columns = <
      item
        Caption = #25991#20214#21517
        Width = 120
      end
      item
        Caption = #25991#20214#36335#24452
        Width = 600
      end>
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    ViewStyle = vsReport
    OnDblClick = ListView1DblClick
    OnKeyDown = ListView1KeyDown
  end
end

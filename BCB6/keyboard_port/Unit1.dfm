object Keyboard: TKeyboard
  Left = 647
  Top = 215
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = #35835#20889#38190#30424#31471#21475
  ClientHeight = 157
  ClientWidth = 324
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 27
    Width = 108
    Height = 13
    Caption = #22312#25991#26412#26694#37324#25353#19979#38190#65306
  end
  object Label2: TLabel
    Left = 64
    Top = 64
    Width = 60
    Height = 13
    Caption = #26102#38388#38388#38548#65306
  end
  object KeyCode: TEdit
    Left = 144
    Top = 24
    Width = 89
    Height = 21
    ReadOnly = True
    TabOrder = 0
    OnKeyDown = KeyCodeKeyDown
  end
  object Interval: TEdit
    Left = 144
    Top = 59
    Width = 89
    Height = 21
    Color = clBtnFace
    TabOrder = 1
  end
  object Start: TButton
    Left = 112
    Top = 112
    Width = 97
    Height = 25
    Caption = #24320#22987
    TabOrder = 2
    OnClick = StartClick
  end
  object down: TCheckBox
    Left = 248
    Top = 24
    Width = 73
    Height = 17
    Caption = 'Down'
    Checked = True
    State = cbChecked
    TabOrder = 3
  end
  object up: TCheckBox
    Left = 248
    Top = 61
    Width = 65
    Height = 17
    Caption = 'Up'
    TabOrder = 4
  end
  object End: TButton
    Left = 232
    Top = 112
    Width = 81
    Height = 25
    Caption = #32467#26463
    TabOrder = 5
    OnClick = EndClick
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 24
    Top = 96
  end
end

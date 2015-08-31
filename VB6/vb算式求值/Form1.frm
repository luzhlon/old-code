VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   4395
   ClientLeft      =   120
   ClientTop       =   450
   ClientWidth     =   7065
   LinkTopic       =   "Form1"
   ScaleHeight     =   4395
   ScaleWidth      =   7065
   StartUpPosition =   3  '窗口缺省
   Begin VB.CommandButton Command1 
      Caption         =   "求值"
      Height          =   735
      Left            =   4440
      TabIndex        =   1
      Top             =   2040
      Width           =   1335
   End
   Begin VB.TextBox Text1 
      Height          =   615
      Left            =   600
      TabIndex        =   0
      Top             =   960
      Width           =   2655
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Declare Function calculate Lib "算式求值dll" (ByVal s As String, ByVal p As String) As Boolean

Private Sub Command1_Click()
Dim b As Boolean
Dim addr As Long
Dim s As String
b = False
s = Text1.Text
addr = StrPtr(s)
b = calculate(s, s)
Text1.Text = s
If b = False Then
 MsgBox ("调用失败")
End If

End Sub


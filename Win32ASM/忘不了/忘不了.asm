;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.386
		.model flat, stdcall
		option casemap :none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Include 文件定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
include		windows.inc
include		user32.inc
includelib	user32.lib
include		kernel32.inc
includelib	kernel32.lib
include		Advapi32.inc
includelib	Advapi32.lib
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Equ 等值定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

EDIT			equ	1
IDD_DIALOG1		equ	102
IDD_NEW		equ	103
IDC_LIST1		equ	1000
IDC_LIST2		equ	1001
IDC_NEW		equ	1002
IDC_DEL		equ	1003
IDC_EDIT		equ	1004
IDC_NAME		equ	1007
IDC_TEXT		equ	1008
IDC_YEAR		equ	1010
IDC_MONTH		equ	1011
IDC_DAY		equ	1012
IDC_HOUR		equ	1014
IDC_MIN		equ	1015

Task	struct

name1	db	100 dup(?)
text	db	1024 dup(?)
year	dd	?
month	dd	?
day	dd	?
hour	dd	?
min	dd	?

Task	ends

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.data?

hRunKey		dd	?
hInstance	dd	?
hAppKey		dd	?
hAppKeyN		dd	?
hWinMian		dd	?
@opration	dd	?
szTskNameBuf	db	8 dup(?)
task			Task	<>
		.const

szCaption   db	"忘不了",0
szSubRunKey		db	"Software\Microsoft\Windows\CurrentVersion\Run",0
szSubAppKey		db	"Software\GoodMemory",0
szEdit			db	"修改任务",0

szName			db	"Name",0
szText			db	"Text",0
szYear			db	"Year",0
szMonth			db	"Month",0
szDay			db	"Day",0
szHour			db	"Hour",0
szMin			db	"Min",0

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

_AutoRun   proc uses eax
            local @szFileName[256]:byte
invoke RegOpenKeyEx,HKEY_LOCAL_MACHINE,offset szSubRunKey,0,KEY_ALL_ACCESS,offset hRunKey
invoke GetModuleFileName,0,addr @szFileName,sizeof @szFileName
inc eax
invoke RegSetValueEx,hRunKey,offset szCaption,0,REG_SZ,addr @szFileName,eax
invoke CloseHandle,hRunKey
ret
_AutoRun   endp



_AddToRegistry   proc

invoke RegCreateKeyEx,hAppKey,offset task.name1,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,offset hAppKeyN,0
invoke RegSetValueEx,hAppKeyN,offset szName,0,0,task.name1,256
invoke RegSetValueEx,hAppKeyN,offset szText,0,0,task.text,1024
invoke RegSetValueEx,hAppKeyN,offset szYear,0,0,task.year,4
invoke RegSetValueEx,hAppKeyN,offset szMonth,0,0,task.month,4
invoke RegSetValueEx,hAppKeyN,offset szDay,0,0,task.day,4
invoke RegSetValueEx,hAppKeyN,offset szHour,0,0,task.hour,4
invoke RegSetValueEx,hAppKeyN,offset szMin,0,0,task.min,4

ret
_AddToRegistry   endp



_DelFromRegistry  proc	pszCurSelName

invoke RegDeleteKey,hAppKey,addr pszCurSelName
ret
_DelFromRegistry   endp



_UpdateListBox   proc 
local dwSize
local lpcbData
local dwIndex
	mov dwIndex,0
	mov lpcbData,256
   .while 1
   	invoke RegEnumKeyEx,hAppKey,dwIndex,offset szTskNameBuf,addr dwSize,0,0,0,0
   	.break .if eax==ERROR_NO_MORE_ITEMS
   	inc dwIndex
   	invoke RegCreateKeyEx,hAppKey,offset szTskNameBuf,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,offset hAppKeyN,0
   	invoke RegQueryValueEx,hAppKeyN,offset szName,0,0,offset task.name1,addr lpcbData
   	invoke SendDlgItemMessage,hWinMian,IDC_LIST1,LB_ADDSTRING,offset task.name1,0
   .endw
   invoke CloseHandle,hAppKey
ret
_UpdateListBox   endp

_ProcDlgNew    proc   hwnd,umsg,wParam,lParam
	local szCurSelName[100]:BYTE
	local dwSize
	local lpcbData
	local @buf
mov eax,umsg
.if eax==WM_CLOSE
exit:
   invoke EndDialog,hwnd,0
.elseif eax==WM_INITDIALOG
	.if @opration==EDIT
		invoke SetWindowText,hwnd,szEdit
		invoke SendDlgItemMessage,hWinMian,IDC_LIST1,LB_GETCURSEL,0,0
		mov @buf,eax
		invoke SendDlgItemMessage,hWinMian,IDC_LIST1,LB_GETTEXT,@buf,addr szCurSelName
		invoke RegCreateKeyEx,hAppKey,addr szCurSelName,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,offset hAppKeyN,0
		invoke RegQueryValueEx,hAppKeyN,offset szName,0,0,offset task.name1,addr lpcbData
		invoke RegQueryValueEx,hAppKeyN,offset szText,0,0,offset task.text,addr lpcbData
		invoke RegQueryValueEx,hAppKeyN,offset szYear,0,0,offset task.year,addr lpcbData
		invoke RegQueryValueEx,hAppKeyN,offset szMonth,0,0,offset task.month,addr lpcbData
		invoke RegQueryValueEx,hAppKeyN,offset szDay,0,0,offset task.day,addr lpcbData
		invoke RegQueryValueEx,hAppKeyN,offset szHour,0,0,offset task.hour,addr lpcbData
		invoke RegQueryValueEx,hAppKeyN,offset szMin,0,0,offset task.min,addr lpcbData
		invoke SetDlgItemText,hwnd,IDC_NAME,offset task.name1
		invoke SetDlgItemText,hwnd,IDC_TEXT,offset task.text
		invoke SetDlgItemInt,hwnd,IDC_YEAR,task.year,0
		invoke SetDlgItemInt,hwnd,IDC_MONTH,task.month,0
		invoke SetDlgItemInt,hwnd,IDC_DAY,task.day,0
		invoke SetDlgItemInt,hwnd,IDC_HOUR,task.hour,0
		invoke SetDlgItemInt,hwnd,IDC_MIN,task.min,0
		invoke CloseHandle,hAppKey
	.endif
.elseif eax==WM_COMMAND
	mov eax,wParam
	.if ax==IDCANCEL
		jmp offset exit
	.elseif ax==IDOK;need to check the buf size
		.if @opration==EDIT
			invoke SendDlgItemMessage,hWinMian,IDC_LIST1,LB_GETCURSEL,0,0
			mov @buf,eax
			invoke SendDlgItemMessage,hWinMian,IDC_LIST1,LB_GETTEXT,@buf,addr szCurSelName
			invoke _DelFromRegistry,addr szCurSelName
		.endif
		invoke GetDlgItemText,hwnd,IDC_NAME,offset Task.name1,256
		invoke GetDlgItemText,hwnd,IDC_TEXT,offset Task.text,1024
		invoke GetDlgItemInt,hwnd,IDC_YEAR,NULL,0
		mov task.year,eax
		invoke GetDlgItemInt,hwnd,IDC_MONTH,NULL,0
		mov task.month,eax
		invoke GetDlgItemInt,hwnd,IDC_DAY,NULL,0
		mov task.day,eax
		invoke GetDlgItemInt,hwnd,IDC_HOUR,NULL,0
		mov task.hour,eax
		invoke GetDlgItemInt,hwnd,IDC_MIN,NULL,0
		mov task.min,eax 
		invoke _AddToRegistry
		jmp offset exit
	.endif
.else
      mov eax,0
      ret
.endif
mov eax,1
ret
_ProcDlgNew   endp

_ProcDlgMain	proc	hWnd,wMsg,wParam,lParam
		local @buf
		local dwSize
		local szCurSelName[100]:BYTE

		mov	eax,wMsg
		.if	eax == WM_CLOSE
			invoke ShowWindow,hWnd,SW_HIDE
		.elseif	eax == WM_INITDIALOG
			push hWnd
			pop hWinMian
			invoke RegCreateKeyEx,HKEY_LOCAL_MACHINE,offset szSubAppKey,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,offset hAppKey,0
			invoke _UpdateListBox
		.elseif	eax == WM_COMMAND
			;***子窗口命令***
			mov	eax,wParam
			.if ax==IDC_NEW
				mov @opration,0
				invoke DialogBoxParam,hInstance,IDD_NEW,hWnd,offset _ProcDlgNew,0
				invoke _UpdateListBox
			.elseif ax==IDC_EDIT
				mov @opration,EDIT
				invoke DialogBoxParam,hInstance,IDD_NEW,hWnd,offset _ProcDlgNew,0
				invoke _UpdateListBox
			.elseif ax==IDC_DEL
				invoke SendDlgItemMessage,hWnd,IDC_LIST1,LB_GETCURSEL,0,0
				mov @buf,eax
				invoke SendDlgItemMessage,hWinMian,IDC_LIST1,LB_GETTEXT,@buf,addr szCurSelName
				invoke _DelFromRegistry,addr szCurSelName
				invoke _UpdateListBox
			.endif
		.else	;其它消息
			mov	eax,FALSE
			ret
		.endif
		mov	eax,TRUE
		ret

_ProcDlgMain	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
start:
		invoke _AutoRun
		invoke	GetModuleHandle,NULL
		mov	hInstance,eax
		invoke DialogBoxParam,hInstance,IDD_DIALOG1,NULL,offset _ProcDlgMain,NULL
		invoke	ExitProcess,NULL
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	start

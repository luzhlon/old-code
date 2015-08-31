;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.386
		.model flat, stdcall
		option casemap :none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Include 文件定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
include		windows.inc
include 		shell32.inc
includelib	shell32.lib
include		user32.inc
includelib	user32.lib
include		kernel32.inc
includelib	kernel32.lib
include		Advapi32.inc
includelib	Advapi32.lib
include		Comdlg32.inc
includelib	Comdlg32.lib
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Equ 等值定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
IDD_DIALOG1		equ	101
IDC_LIST		equ	1000
IDC_INFO		equ	1001
IDC_IMM		equ	1002
IDC_CHECK1		equ	1003
IDC_OPEN		equ	1004
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.data?

hInstance	dd	?
hWinMain	dd	?
hSubKey	dd	?
hKey		dd	?
		.const
szDian	db	".",0
szSubKey	db	"SoftWare\OpenEasy",0
szFilter	db	"所有文件",0,"*.*",0,0
szOprea	db	"open",0
szExe		db	"open.exe",0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Take the filename
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_TakeFileName		proc	lpszFile,lpszFileName
		pushad
		mov ebx,lpszFile											;字符串基地址
		xor eax,eax
		.while 1														;字符串长度
			mov cl,[ebx+eax]
			.break .if cl==0
			inc eax
		.endw
		.while 1														;找到'\'的偏移量
			mov cl,[ebx+eax]
			.break .if cl=='\'
			dec eax
			.if eax==0												;若没有，则返回0
				ret
			.endif
		.endw
		inc eax
		mov esi,lpszFileName
		xor edx,edx
		.while 1														;复制'\'后面的字符串
			mov cl,[ebx+eax]
			mov [esi+edx],cl
			.break .if cl==0
			inc eax
			inc edx
		.endw
		popad
		mov eax,1
		ret
_TakeFileName		endp

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 通用对话框  打开文件
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_OpenFile		proc	lpszFilter,lpszFile
		local @stOF:OPENFILENAME
		
		invoke RtlZeroMemory,addr @stOF,sizeof @stOF
		invoke RtlZeroMemory,lpszFile,MAX_PATH
		mov @stOF.lStructSize,sizeof @stOF
		push hWinMain
		pop  @stOF.hwndOwner
		push lpszFilter
		pop  @stOF.lpstrFilter													;过滤文件
		mov eax,lpszFile
		mov @stOF.lpstrFile,eax													;文件路径名
		mov  @stOF.nMaxFile,MAX_PATH
		mov  @stOF.lpstrFileTitle,NULL										;文件名
		mov  @stOF.Flags,OFN_FILEMUSTEXIST  or OFN_PATHMUSTEXIST
		invoke GetOpenFileName,addr @stOF
		ret
_OpenFile		endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;添加到注册表
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_AddToRegistry  proc			lpszFile
local hAppKeyN
local dwIndex
local dwSize
local szFileName[MAX_PATH]:BYTE
local szBuf[MAX_PATH]:BYTE

invoke _TakeFileName,lpszFile,addr szFileName
	mov dwIndex,0
	 .while 1
		mov dwSize,sizeof szBuf
		invoke RegEnumKeyEx,hKey,dwIndex,addr szBuf,addr dwSize,0,0,0,0
		.break .if eax==ERROR_NO_MORE_ITEMS
		invoke lstrcmp,addr szFileName,addr szBuf											;同名检查处理
		.if eax==0
			invoke lstrcat,addr szFileName,offset szDian
		.endif
		inc dwIndex
   .endw
invoke RegCreateKeyEx,hKey,addr szFileName,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,addr hAppKeyN,0
invoke RegSetValueEx,hAppKeyN,0,0,REG_SZ,lpszFile,MAX_PATH
invoke CloseHandle,hAppKeyN
ret
_AddToRegistry   endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;更新列表框
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_UpdateListBox   proc 
	local dwSize
	local lpcbData
	local dwIndex
	local hAppKeyN
	local szBuf[MAX_PATH]:BYTE
	mov dwIndex,0
	mov lpcbData,256
	invoke SendDlgItemMessage,hWinMain,IDC_LIST,LB_RESETCONTENT,0,0			;清除列表框原来的内容
   .while 1
   	mov dwSize,sizeof szBuf
   	invoke RegEnumKeyEx,hKey,dwIndex,addr szBuf,addr dwSize,0,0,0,0
   	.break .if eax==ERROR_NO_MORE_ITEMS
   	inc dwIndex
   	invoke SendDlgItemMessage,hWinMain,IDC_LIST,LB_ADDSTRING,0,addr szBuf
   .endw
	ret
_UpdateListBox   endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_ProcDlgMain	proc	uses ebx edi esi hWnd,wMsg,wParam,lParam
		;****局部变量****
		local szFile[MAX_PATH]:BYTE
		local szListText[MAX_PATH]:BYTE
		local dwData
		local hAppKeyN
		
		mov	eax,wMsg
		.if	eax == WM_CLOSE
			;***退出时代码***
			invoke EndDialog,hWnd,0
		.elseif	eax == WM_INITDIALOG
			;***初始化代码***
			push hWnd
			pop hWinMain
			;打开注册表相应子键
			invoke RegCreateKeyEx,HKEY_LOCAL_MACHINE,offset szSubKey,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,offset hKey,0
			invoke _UpdateListBox
		.elseif	eax == WM_COMMAND
			;***子窗口命令***
			mov	eax,wParam
			.if	ax ==	IDCANCEL
				invoke SendDlgItemMessage,hWnd,IDC_LIST,LB_GETCURSEL,0,0
				mov ebx,eax
				invoke SendDlgItemMessage,hWnd,IDC_LIST,LB_GETTEXT,ebx,addr szListText
				invoke RegDeleteKey,hKey,addr szListText
				invoke _UpdateListBox
			.elseif	ax ==	IDOK
				invoke _OpenFile,offset szFilter,addr szFile
				.if eax==0
					jmp @f
				.endif
				invoke _AddToRegistry,addr szFile
				invoke _UpdateListBox
			.elseif 	ax==IDC_OPEN
				invoke SendDlgItemMessage,hWnd,IDC_LIST,LB_GETCURSEL,0,0
				mov ebx,eax
				invoke SendDlgItemMessage,hWnd,IDC_LIST,LB_GETTEXT,ebx,addr szListText
				invoke RegOpenKeyEx,hKey,addr szListText,0,KEY_ALL_ACCESS,addr hAppKeyN
				mov dwData,MAX_PATH
				invoke RegQueryValueEx,hAppKeyN,NULL,0,NULL,addr szFile,addr dwData
				invoke RegCloseKey,hAppKeyN
				invoke ShellExecute,hWnd,offset szOprea,addr szFile,0,0,SW_NORMAL
			.elseif ax==IDC_IMM
				invoke ShellExecute,hWnd,offset szOprea,offset szExe,0,0,SW_NORMAL
			.elseif ax==IDC_LIST
				shr	eax,16
				.if ax==LBN_SELCHANGE
					invoke SendDlgItemMessage,hWnd,IDC_LIST,LB_GETCURSEL,0,0
					mov ebx,eax
					invoke SendDlgItemMessage,hWnd,IDC_LIST,LB_GETTEXT,ebx,addr szListText
					invoke RegOpenKeyEx,hKey,addr szListText,0,KEY_ALL_ACCESS,addr hAppKeyN
					mov dwData,MAX_PATH
					invoke RegQueryValueEx,hAppKeyN,NULL,0,NULL,addr szFile,addr dwData
					invoke RegCloseKey,hAppKeyN
					invoke SetDlgItemText,hWnd,IDC_INFO,addr szFile
				.endif
			.endif
		.else
			mov	eax,FALSE
			ret
		.endif
	@@:mov	eax,TRUE
		ret

_ProcDlgMain	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
start:
		invoke	GetModuleHandle,NULL
		mov	hInstance,eax
		invoke	DialogBoxParam,hInstance,IDD_DIALOG1,NULL,offset _ProcDlgMain,NULL
		invoke	ExitProcess,NULL
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	start

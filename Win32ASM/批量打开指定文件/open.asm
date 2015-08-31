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

IDD_DIALOG1		equ	101

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.data?
hInstance	dd	?
hWinMain		dd	?
hKey		dd	?
		.const
szSubKey	db	"SoftWare\OpenEasy",0
szOprea	db	"open",0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_EnumAndOpen	proc 
		local hAppKeyN
		local dwIndex
		local dwSize
		local szFile[MAX_PATH]:BYTE
		local szBuf[MAX_PATH]:BYTE

		;枚举子健
		mov dwIndex,0
		.while 1
			mov dwSize,sizeof szBuf
			invoke RegEnumKeyEx,hKey,dwIndex,addr szBuf,addr dwSize,0,0,0,0
			.break .if eax==ERROR_NO_MORE_ITEMS
			invoke RegOpenKeyEx,hKey,addr szBuf,0,KEY_ALL_ACCESS,addr hAppKeyN
			mov dwSize,MAX_PATH
			invoke RegQueryValueEx,hAppKeyN,NULL,0,NULL,addr szFile,addr dwSize
			invoke RegCloseKey,hAppKeyN
			invoke ShellExecute,hWinMain,offset szOprea,addr szFile,0,0,SW_NORMAL
			inc dwIndex
		.endw
		ret
_EnumAndOpen	endp

_ProcDlgMain	proc	uses ebx edi esi hWnd,wMsg,wParam,lParam
		
		mov	eax,wMsg
		.if	eax == WM_CLOSE
			;***退出时代码***
			invoke RegCloseKey,hKey
			invoke EndDialog,hWnd,0
		.elseif	eax == WM_INITDIALOG
			push hWnd
			pop hWinMain
			;打开注册表相应子键
			invoke RegCreateKeyEx,HKEY_LOCAL_MACHINE,offset szSubKey,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,offset hKey,0
			invoke _EnumAndOpen
			invoke RegCloseKey,hKey
			invoke EndDialog,hWnd,0
		.else
			mov	eax,FALSE
			ret
		.endif
	@@:mov	eax,TRUE
		ret

_ProcDlgMain	endp

start:
		invoke	GetModuleHandle,NULL
		mov	hInstance,eax
		invoke	DialogBoxParam,hInstance,IDD_DIALOG1,NULL,offset _ProcDlgMain,NULL
		invoke	ExitProcess,NULL
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	start

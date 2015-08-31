;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.386
		.model flat, stdcall
		option casemap :none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Include 文件定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
include		windows.inc
include		shell32.inc
includelib	shell32.lib
include		user32.inc
includelib	user32.lib
include		kernel32.inc
includelib	kernel32.lib
include		Advapi32.inc
includelib	Advapi32.lib
include		Hookdll.inc
includelib	Hookdll.lib
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Equ 等值定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
IDD_MIAN		equ	101
WM_HOOK		equ	WM_USER+5

rest	struct
	frt		dd	?
	Count		dd	?
rest		ends
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.data?
hWinMain		dd	?
hInstance	dd	?
hKey		dd	?
rt			rest	<>
		.const
szFrt		db	"SoftWare\Forcible rt",0
szrt		db	"rt?",0
szCount		db	"Count",0
szSubRunKey	db	"Software\Microsoft\Windows\CurrentVersion\Run",0
szScrFile	db	"Nature_3D_Screensaver\Nature 3D Screensaver.exe",0
szOprea		db	"open",0

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 开机自启动
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_AutoRun  proc	uses eax
local @szFileName[256]:BYTE
local hRunKey
invoke RegOpenKeyEx,HKEY_LOCAL_MACHINE,offset szSubRunKey,0,KEY_ALL_ACCESS,addr hRunKey
invoke GetModuleFileName,0,addr @szFileName,sizeof @szFileName
inc eax
invoke RegSetValueEx,hRunKey,offset szFrt,0,REG_SZ,addr @szFileName,eax
invoke CloseHandle,hRunKey
ret
_AutoRun   endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 从注册表读入
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_Read		proc	
	local dwbuf
	local cbdata
	
	mov cbdata,4
	invoke RegQueryValueEx,hKey,offset szrt,0,NULL,addr dwbuf,addr cbdata
	.if eax==ERROR_SUCCESS
		push dwbuf
		pop rt.frt
	.else
		mov eax,0
		ret
	.endif
	
	mov cbdata,4
	invoke RegQueryValueEx,hKey,offset szCount,0,NULL,addr dwbuf,addr cbdata
	.if eax==ERROR_SUCCESS
		push dwbuf
		pop rt.Count
	.else
		mov eax,0
		ret
	.endif
	mov eax,1
	ret
_Read		endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 向注册表写入
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_Write	proc	

	invoke RegSetValueEx,hKey,offset szrt,0,REG_DWORD,offset rt.frt,4
	invoke RegSetValueEx,hKey,offset szCount,0,REG_DWORD,offset rt.Count,4
	ret
_Write	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 计时
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_ProcTimerCount	proc	uses ebx edi esi hWnd,uMsg,idEvent,dwTime

	invoke _Read
	.if rt.Count
		dec rt.Count
		invoke _Write
	.else
		not rt.frt
		.if rt.frt
			mov rt.Count,10
			invoke _Write
			invoke SendMessage,hWinMain,WM_USER+1,0,0
		.else
			invoke SendMessage,hWinMain,WM_USER+2,0,0
			mov rt.Count,40
			invoke _Write
		.endif
	.endif
	ret
_ProcTimerCount	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_ProcDlgMain	proc	uses ebx edi esi hWnd,wMsg,wParam,lParam
		mov	eax,wMsg
		.if	eax == WM_CLOSE
			;***退出时代码***
			invoke ShowWindow,hWnd,SW_HIDE
		.elseif	eax == WM_INITDIALOG
			push hWnd
			pop hWinMain
			;打开注册表键  供程序试用
			invoke RegCreateKeyEx,HKEY_LOCAL_MACHINE,offset szFrt,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,offset hKey,0
			;查询键值rt？、count  失败则创建
			invoke _Read
			.if !eax
				mov rt.frt,0
				mov rt.Count,40
			.endif
			.if !rt.frt
				mov rt.Count,40
			.endif
			invoke _Write
			invoke SetTimer,NULL,0,60000,offset _ProcTimerCount
			invoke SetTimer,hWnd,1,100,NULL
		.elseif eax==WM_TIMER
			invoke ShowWindow,hWnd,SW_HIDE
			invoke KillTimer,hWnd,1
		.elseif eax==WM_USER+1
			invoke ShowWindow,hWnd,SW_NORMAL or SW_RESTORE
			invoke SetWindowPos,hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE				;设置顶层窗口
			invoke ShellExecute,hWnd,offset szOprea,offset szScrFile,0,0,SW_SHOWNORMAL
			invoke	InstallHook,hWnd,WM_HOOK										;安装键盘钩子
		.elseif eax==WM_USER+2
			invoke ShowWindow,hWnd,SW_HIDE
			invoke keybd_event,VK_ESCAPE,0,KEYEVENTF_KEYUP,0
			invoke	UninstallHook													;卸载键盘钩子
		.else	;其它消息
			mov	eax,FALSE
			ret
		.endif
		mov	eax,TRUE
		ret

_ProcDlgMain	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
start:
		invoke	GetModuleHandle,NULL
		mov	hInstance,eax
		invoke   _AutoRun
		invoke	DialogBoxParam,hInstance,IDD_MIAN,NULL,offset _ProcDlgMain,NULL
		invoke	ExitProcess,NULL
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	start
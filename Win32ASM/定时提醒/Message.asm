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

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Equ 等值定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
IDD_MAIN		equ	101
IDD_MSG		equ	102
IDI_ICON1	equ	103
IDC_TEXT		equ	1000
IDC_MIN		equ	1001
IDC_SECOND	equ	1002
IDC_EDIT1	equ	1003
IDT_MAIN		equ	1
IDT_SHOW		equ	2
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.data?

hInstance	dd	?
hWndShow		dd	?
dwMin			dd	?
dwSecond		dd	?

szText		db	256 dup(?)
		.data

		.const
szNULL		db	0
szError1		db	"时间格式不对",0
szError2		db	"文本过长",0
szErrCap		db	"错误",0

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_ProcDlgShow	proc	uses ebx edi esi hWnd,wMsg,wParam,lParam
		mov	eax,wMsg
		.if	eax == WM_INITDIALOG
			invoke SetDlgItemText,hWnd,IDC_EDIT1,offset szText
			invoke	SetWindowPos,hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE
			invoke SetTimer,hWnd,IDT_SHOW,60000,NULL
			invoke GetDlgItem,hWnd,IDOK
			invoke EnableWindow,eax,FALSE
		.elseif	eax==WM_TIMER
			invoke KillTimer,hWnd,IDT_SHOW
			invoke GetDlgItem,hWnd,IDOK
			invoke EnableWindow,eax,TRUE
		.elseif	eax==WM_COMMAND
			mov eax,wParam
			.if ax==IDOK
				invoke	EndDialog,hWnd,0
			.endif
		.else	;其它消息
			mov	eax,FALSE
			ret
		.endif
		@@:	mov	eax,TRUE
		ret
_ProcDlgShow	endp


_ProcDlgMain	proc	uses ebx edi esi hWnd,wMsg,wParam,lParam
		local @sf

		mov	eax,wMsg
		.if	eax == WM_INITDIALOG
			;***初始化代码***
			invoke LoadIcon,hInstance,IDI_ICON1
			invoke SendMessage,hWnd,WM_SETICON,ICON_BIG,eax
			invoke SetDlgItemInt,hWnd,IDC_MIN,0,0
			invoke SetDlgItemInt,hWnd,IDC_SECOND,0,0
		.elseif	eax == WM_COMMAND
			;***子窗口命令***
			mov	eax,wParam
			.if	ax ==	IDCANCEL
				invoke SetDlgItemText,hWnd,IDC_TEXT,offset szNULL
				invoke SetDlgItemInt,hWnd,IDC_MIN,0,0
				invoke SetDlgItemInt,hWnd,IDC_SECOND,0,0
			.elseif	ax ==	IDOK
				invoke GetDlgItemText,hWnd,IDC_TEXT,offset szText,256
				.if	eax>=256
					invoke MessageBox,hWnd,offset szError2,offset szErrCap,MB_OK
					jmp @f
				.endif
				invoke GetDlgItemInt,hWnd,IDC_MIN,addr @sf,0
				mov dwMin,eax
				invoke GetDlgItemInt,hWnd,IDC_SECOND,addr @sf,0
				mov dwSecond,eax
				.if	eax>=60
					invoke MessageBox,hWnd,offset szError1,offset szErrCap,MB_OK
					jmp @f
				.endif
				mov eax,60
				mul dwMin
				add dwSecond,eax
				mov eax,1000
				mul dwSecond
				invoke SetTimer,hWnd,IDT_MAIN,eax,0
				invoke GetDlgItem,hWnd,IDOK
				invoke EnableWindow,eax,FALSE
			.endif
		.elseif	eax==WM_TIMER
			invoke	GetDlgItem,hWnd,IDOK
			invoke	EnableWindow,eax,TRUE
			invoke	KillTimer,hWnd,IDT_MAIN
			invoke	DialogBoxParam,hInstance,IDD_MSG,NULL,offset _ProcDlgShow,NULL
		.else	;其它消息
			mov	eax,FALSE
			ret
		.endif
@@:	mov	eax,TRUE
		ret
_ProcDlgMain	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
start:
		invoke	GetModuleHandle,NULL
		mov	hInstance,eax
		invoke	DialogBoxParam,hInstance,IDD_MAIN,NULL,offset _ProcDlgMain,NULL
		invoke	ExitProcess,NULL
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	start

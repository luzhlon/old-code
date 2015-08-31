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
include		gdi32.inc
includelib	gdi32.lib
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Equ 等值定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
IDD_DIALOG1		equ		101
IDC_IN			equ		1000
IDC_OUT			equ		1001
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.data?
hInstance	dd	?
hWinMain	dd	?

		.const
num		db	0Ch,0Ah,13h,09h,0Ch,0Bh,0Ah,08h
szFmt		db	"%ld",0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_ProcDlgMain	proc	uses ebx edi esi hWnd,wMsg,wParam,lParam
		;****局部变量****
		local szName[MAX_PATH]:BYTE
		local szCode[MAX_PATH]:BYTE
		local nCode
		local len
		mov	eax,wMsg
		.if	eax == WM_CLOSE
			;***退出时代码***
			invoke EndDialog,hWnd,0
		.elseif	eax == WM_COMMAND
			;***子窗口命令***
			mov	eax,wParam
			.if	ax ==	IDOK
				invoke GetDlgItemText,hWnd,IDC_IN,addr szName,MAX_PATH
				mov len,eax
				mov nCode,0
				mov ecx,3
				lea ebx,szName
				lea edi,num
				.while ecx<len
					xor eax,eax
					xor edx,edx
					mov al,BYTE ptr [edi]
					mov dl,BYTE ptr [ecx+ebx]
					imul eax,edx
					add nCode,eax
					inc ecx
					inc edi
				.endw
				invoke wsprintf,addr szCode,offset szFmt,nCode
				invoke SetDlgItemText,hWnd,IDC_OUT,addr szCode
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
		invoke	GetModuleHandle,NULL
		mov	hInstance,eax
		invoke	DialogBoxParam,hInstance,IDD_DIALOG1,NULL,offset _ProcDlgMain,NULL
		invoke	ExitProcess,NULL
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	start

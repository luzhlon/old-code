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
include		Comdlg32.inc
includelib	Comdlg32.lib
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Equ 等值定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
IDD_DIALOG1		equ	101
IDC_EDIT1		equ	1000
IDC_EDIT2		equ	1001
IDC_BUTTON1		equ	1002
IDC_BUTTON2		equ	1003

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.data?

hInstance	dd	?
hWinMain		dd	?
hFile1		dd	?
hFile2		dd	?
hFile3		dd	?
szFilePath	db	200 dup(?)

		.const
szCap		db	"提示",0
szFileErr	db	"创建文件失败",0
szFilter	db	"所有文件",0,"*.*",0,0
szFileName	db	"合并后文件",0
szOk			db	"合并完成！",0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 返回文件路径字符串
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_OpenFile	proc	lpszFilter,lpszFilePath
	local	@stOF:OPENFILENAME
	local	@szBuf[200]:BYTE

	invoke RtlZeroMemory,addr @stOF,sizeof @stOF
	mov @stOF.lStructSize,sizeof @stOF
	push hWinMain
	pop  @stOF.hwndOwner
	push lpszFilter
	pop  @stOF.lpstrFilter									;过滤文件
	push lpszFilePath
	pop  @stOF.lpstrFile											;文件路径名
	mov  @stOF.nMaxFile,MAX_PATH
	lea eax,@szBuf
	mov  @stOF.lpstrFileTitle,eax						;文件名
	mov  @stOF.Flags,OFN_FILEMUSTEXIST  or OFN_PATHMUSTEXIST
	invoke GetOpenFileName,addr @stOF
	ret
_OpenFile	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_ProcDlgMain	proc	uses ebx edi esi hWnd,wMsg,wParam,lParam
		;****局部变量****
		local szBuf[100]:BYTE
		local	nRead
		mov	eax,wMsg
		.if	eax == WM_CLOSE
			;***退出时代码***
			invoke EndDialog,hWnd,0
		.elseif	eax == WM_INITDIALOG
			;***初始化代码***
		.elseif	eax == WM_COMMAND
			;***子窗口命令***
			mov	eax,wParam
			.if	ax ==	IDCANCEL
				invoke EndDialog,hWnd,0
			.elseif	ax ==	IDOK
				;打开副文件
				invoke GetDlgItemText,hWnd,IDC_EDIT2,offset szFilePath,sizeof szFilePath
				invoke CreateFile,offset szFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL
				mov hFile2,eax
				.if eax==INVALID_HANDLE_VALUE													;文件创建失败
					invoke MessageBox,hWinMain,offset szFileErr,offset szCap,NULL
					jmp @f
				.endif
				;打开主文件
				invoke GetDlgItemText,hWnd,IDC_EDIT1,offset szFilePath,sizeof szFilePath
				invoke CreateFile,offset szFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL
				mov hFile1,eax
				.if eax==INVALID_HANDLE_VALUE													;文件创建失败
					invoke MessageBox,hWinMain,offset szFileErr,offset szCap,NULL
					jmp @f
				.endif
				;创建新文件
				invoke lstrcat,offset szFilePath,offset szFileName
				invoke CreateFile,offset szFilePath,GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL
				mov hFile3,eax
				.if eax==INVALID_HANDLE_VALUE													;文件创建失败
					invoke MessageBox,hWinMain,offset szFileErr,offset szCap,NULL
					jmp @f
				.endif
				;写入新文件  主文件的数据
				.while TRUE
					invoke ReadFile,hFile1,addr szBuf,100,addr nRead,NULL
					.break .if nRead==0
					invoke WriteFile,hFile3,addr szBuf,100,addr nRead,NULL
				.endw
				;写入新文件  副文件的数据
				.while TRUE
					invoke ReadFile,hFile2,addr szBuf,100,addr nRead,NULL
					.break .if nRead==0
					invoke WriteFile,hFile3,addr szBuf,100,addr nRead,NULL
				.endw
				invoke CloseHandle,hFile1
				invoke CloseHandle,hFile2
				invoke CloseHandle,hFile3
				invoke MessageBox,hWnd,offset szOk,offset szCap,MB_OK
			.elseif	ax==IDC_BUTTON1	;其它子窗口命令
				invoke _OpenFile,offset szFilter,offset szFilePath
				invoke SetDlgItemText,hWnd,IDC_EDIT1,offset szFilePath
			.elseif ax==IDC_BUTTON2
				invoke _OpenFile,offset szFilter,offset szFilePath
				invoke SetDlgItemText,hWnd,IDC_EDIT2,offset szFilePath
			.endif
		.else	;其它消息
		@@:mov	eax,FALSE
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

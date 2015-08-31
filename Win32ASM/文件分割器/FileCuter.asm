	.386
	.model flat,stdcall
	option casemap:none
	include		windows.inc
	include		user32.inc
	includelib	user32.lib
	include		kernel32.inc
	includelib	kernel32.lib
	include		Comdlg32.inc
	includelib	Comdlg32.lib
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;equ等值定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
IDD_DIALOG1	equ	101
IDC_PATH	equ	1004
IDC_BYTES	equ	1001
IDC_SELECT	equ	1005
IDC_KB		equ	1000
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	.data?
hInsWnd		dd	?
hFile			dd	?
hFile1		dd	?
hFile2		dd	?
hInstance	dd	?
hWinMain		dd	?
szFileName	db	256 dup(?)
szPathName1	db	256 dup(?)
szPathName2	db	256 dup(?)
szBuffer	db	1024 dup(?)
szPath		db	MAX_PATH dup (?)
dwFileLength	dd	?
dwBytes		dd	?
dwCurPoint	dd	?
dwKB			dd	?
	.data

	.const
szCutErr	db	'请检查分割长度',0
szErr		db	'请输入正确的文件路径名！',0
szCap		db	'提示',0
szFilter	db	'Text File(*.txt)',0,'*.txt',0
		db	'All Files(*.*)',0,'*.*',0,0
szDefExt	db	'txt',0
szN1		db	'文件_1',0
szN2		db	'文件_2',0
szFormat	db	'%s%s',0
szOk		db	'分割完成',0
szFileErr	db	'创建文件失败',0

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;选择文件	子过程
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_OpenFile	proc	
	local	@stOF:OPENFILENAME

	invoke RtlZeroMemory,addr @stOF,sizeof @stOF
	mov @stOF.lStructSize,sizeof @stOF
	push hWinMain
	pop  @stOF.hwndOwner
	mov  @stOF.lpstrFilter,offset szFilter								;过滤文件
	mov  @stOF.lpstrFile,offset szPath									;文件路径名
	mov  @stOF.nMaxFile,MAX_PATH
	mov  @stOF.lpstrFileTitle,offset szFileName						;文件名
	mov  @stOF.Flags,OFN_FILEMUSTEXIST  or OFN_PATHMUSTEXIST
	invoke GetOpenFileName,addr @stOF
	invoke SetDlgItemText,hWinMain,IDC_PATH,offset szPath

	ret

_OpenFile	endp

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;切割文件	子过程
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_CutFile	proc	uses ecx
	
	local NumOfRead
	local NumOfWrite
			;获取分割参数
			invoke GetDlgItemInt,hInsWnd,IDC_BYTES,NULL,FALSE
			mov dwBytes,eax
			invoke GetDlgItemInt,hInsWnd,IDC_KB,NULL,FALSE
			mov dwKB,eax
;格式化文件名
	invoke wsprintf,offset szPathName1,offset szFormat,offset szPath,offset szN1
	invoke wsprintf,offset szPathName2,offset szFormat,offset szPath,offset szN2
;创建文件缓冲  文件1 文件2
	invoke CreateFile,offset szPathName1,GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL
	mov hFile1,eax
	.if eax==INVALID_HANDLE_VALUE													;文件创建失败
		invoke MessageBox,hWinMain,offset szFileErr,offset szCap,NULL
	.endif
	invoke CreateFile,offset szPathName2,GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL
	mov hFile2,eax
	.if eax==INVALID_HANDLE_VALUE													;文件创建失败
		invoke MessageBox,hWinMain,offset szFileErr,offset szCap,NULL
	.endif

;向文件1写入数据
	.while dwKB!=0
			invoke ReadFile,hFile,offset szBuffer,1024,addr NumOfRead,NULL
			invoke WriteFile,hFile1,offset szBuffer,1024,addr NumOfWrite,NULL
            dec ecx
            dec dwKB
	.endw
   
	.while dwBytes!=0
            invoke ReadFile,hFile,offset szBuffer,1,addr NumOfRead,NULL
			invoke WriteFile,hFile1,offset szBuffer,1,addr NumOfWrite,NULL
            dec ecx
            dec dwBytes
	.endw
;向文件2中写入数据
	.while TRUE
				invoke ReadFile,hFile,offset szBuffer,1024,addr NumOfRead,NULL
				.break .if NumOfRead==0
				invoke WriteFile,hFile2,offset szBuffer,NumOfRead,addr NumOfWrite,NULL
	.endw
;完成提示
	invoke MessageBox,hInsWnd,offset szOk,offset szCap,MB_OK
;关闭文件句柄
					invoke CloseHandle,hFile1
					invoke CloseHandle,hFile2
	ret
_CutFile	endp


;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;对话框过程
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_ProcDlgMain	proc	hWnd,wMsg,wParam,lParam
	pushad
	push	hWnd
	pop	hInsWnd
	mov eax,wMsg
	.if	eax==WM_CLOSE
		invoke	EndDialog,hWnd,1
	;初始化窗口
	.elseif	eax==WM_INITDIALOG
		push hWnd
		pop  hWinMain
		invoke SetDlgItemInt,hWnd,IDC_BYTES,0,0
	.elseif	eax==WM_COMMAND
		mov eax,wParam
		.if	eax==IDC_SELECT
			invoke _OpenFile
		.elseif	eax==IDOK
			;打开文件
			invoke GetDlgItemText,hWinMain,IDC_PATH,offset szPath,MAX_PATH
			invoke CreateFile,offset szPath,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL
			.if eax==INVALID_HANDLE_VALUE
				invoke MessageBox,hWnd,offset szErr,offset szCap,MB_OK
				mov eax,TRUE
				ret
			.endif
			mov hFile,eax
			invoke _CutFile
			invoke CloseHandle,hFile
		.endif
	.else	
		popad
		mov eax,FALSE
		ret
	.endif
	popad
	mov eax,TRUE
	ret
_ProcDlgMain	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;程序入口
start:
	invoke GetModuleHandle,NULL
	mov hInstance,eax
	invoke DialogBoxParam,hInstance,IDD_DIALOG1,NULL,offset _ProcDlgMain,NULL
	invoke ExitProcess,NULL
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	end	start


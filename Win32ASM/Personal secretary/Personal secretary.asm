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
include		Comdlg32.inc
includelib	Comdlg32.lib

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; Equ 等值定义
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
EDIT			equ	1
NEW			equ	0

ADATE			equ	000h
AWEEK			equ	100h
NDLG			equ	200h

WM_ICON		equ	WM_USER+5

IDD_MAIN		equ	101
IDD_NEW		equ	102
IDD_SHOW		equ	103
IDI_ICON1	equ	104
IDC_LIST		equ	1000
IDC_TEXT		equ	1001
IDC_NEW		equ	1002
IDC_EDIT		equ	1003
IDC_DEL		equ	1004
IDC_ABOUT		equ	1005
IDC_ADATE		equ	1015
IDC_AWEEK		equ	1016
IDC_NAME		equ	1020
IDC_YEAR		equ	1024
IDC_MONTH		equ	1025
IDC_DAY		equ	1026
IDC_HOUR		equ	1027
IDC_MIN		equ	1028
IDC_CDSECONDS		equ	1032
IDC_FILEPATH		equ	1033
IDC_SELFILE		equ	1034
IDC_TskTEXT		equ	1035
IDC_CHECK7		equ	1057
IDC_CHECK1		equ	1051
IDC_CHECK2		equ	1052
IDC_CHECK3		equ	1053
IDC_CHECK4		equ	1054
IDC_CHECK5		equ	1055
IDC_CHECK6		equ	1056
IDC_ISDIALOG	equ	1043
IDC_TXTSHOW		equ	1008
IDC_NAMESHOW   equ   1044
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 任务结构体
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Task	struct
name1	db	22 dup(?)
text	db	202 dup(?)
appath	db	200 dup(?)
year	dd	?
month	dd	?
day	dd	?
hour	dd	?
min	dd	?
ttype	dd	?

Task	ends
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 数据段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.data?
hInstance	dd	?
hWinMain		dd	?
hRunKey		dd	?
hAppKey		dd	?
hAppKeyN		dd	?
hKeyList		dd	?
hKeyDate		dd	?
hKeyXingqi	dd	?
@opration	dd	?
lpOldProc	dd	?
szBuf			dd	200 dup(?)
task			Task		<>
		.data
bTest			dd	1b,10b,100b,1000b,10000b,100000b,1000000b
idTimer		dd 0
bShow			dd	0
		.const
szCaption		db	"Personal secretary ",0
szSubRunKey		db	"Software\Microsoft\Windows\CurrentVersion\Run",0
szKeyList		db	"Software\GoodMemory\list",0
szKeyDate		db	"Software\GoodMemory\index\date",0
szKeyXingqi		db	"Software\GoodMemory\index\xingqi",0
szEdit			db	"修改任务",0

szName			db	"Name",0
szText			db	"Text",0
szYear			db	"Year",0
szMonth			db	"Month",0
szDay				db	"Day",0
szHour			db	"Hour",0
szMin				db	"Min",0
szType			db	"type",0
szPath			db	"path",0
szMsgCap			db	"提示",0
szSameName		db	"存在同名任务！",0
szError1			db	"任务名必须小于10个汉字!",0
szError2			db	"任务内容必须小于100个汉字!",0
szError3			db	"注册表操作失败!",0
szError4			db	"任务名不能为空！",0
szTimeErr		db	"时间格式错误",0
szAboutCap		db	"关于",0
szAbout			db	"作者：Code Soul",10,13,
						"版本：V1.0",10,13,
						"2013.6.17",0
szFmt				db	"任务名称:%s",13,10,
						"任务内容：%s",13,10,
						"执行路径：%s",13,10,
						"提醒时间：",0
szFmtData		db	"%d年%d月%d日%d时%d分",0
szFmtTime		db	"%d时%d分",13,10,0
szFmtWeek		db	"	周一",13,10,0,
						"	周二",13,10,0,
						"	周三",13,10,0,
						"	周四",13,10,0,
						"	周五",13,10,0,
						"	周六",13,10,0,
						"	周日",0

szIcon			db	"双击打开Personal secretary",0
szFmtKeyD		db	"%d\%d\%d\%d\%d",0
szFmtKeyZ		db	"%d\%d\%d",0
szFilter			db	"应用程序(*.exe)",0,"*.exe",0,0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码段
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;查询键值to task结构
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_QueryValue	proc	pszToQuery
	local lpcbData
	invoke RegOpenKeyEx,hKeyList,pszToQuery,0,KEY_ALL_ACCESS,offset hAppKeyN
	.if eax==ERROR_SUCCESS
		invoke RegQueryValueEx,hAppKeyN,offset szName,0,NULL,offset task.name1,addr lpcbData
		mov lpcbData,202
		invoke RegQueryValueEx,hAppKeyN,offset szText,0,NULL,offset task.text,addr lpcbData
		invoke RegQueryValueEx,hAppKeyN,offset szYear,0,NULL,offset task.year,addr lpcbData
		invoke RegQueryValueEx,hAppKeyN,offset szMonth,0,NULL,offset task.month,addr lpcbData
		invoke RegQueryValueEx,hAppKeyN,offset szDay,0,NULL,offset task.day,addr lpcbData
		invoke RegQueryValueEx,hAppKeyN,offset szHour,0,NULL,offset task.hour,addr lpcbData
		invoke RegQueryValueEx,hAppKeyN,offset szMin,0,NULL,offset task.min,addr lpcbData
		invoke RegQueryValueEx,hAppKeyN,offset szType,0,NULL,offset task.ttype,addr lpcbData
	.endif
	invoke CloseHandle,hAppKeyN
	ret
_QueryValue	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;添加到注册表
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_AddToRegistry  proc	uses eax
invoke RegCreateKeyEx,hKeyList,offset task.name1,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,offset hAppKeyN,0
invoke RegSetValueEx,hAppKeyN,offset szName,0,REG_SZ,offset task.name1,22
invoke RegSetValueEx,hAppKeyN,offset szText,0,REG_SZ,offset task.text,202
invoke RegSetValueEx,hAppKeyN,offset szPath,0,REG_SZ,offset task.appath,200
invoke RegSetValueEx,hAppKeyN,offset szYear,0,REG_DWORD,offset task.year,4
invoke RegSetValueEx,hAppKeyN,offset szMonth,0,REG_DWORD,offset task.month,4
invoke RegSetValueEx,hAppKeyN,offset szDay,0,REG_DWORD,offset task.day,4
invoke RegSetValueEx,hAppKeyN,offset szHour,0,REG_DWORD,offset task.hour,4
invoke RegSetValueEx,hAppKeyN,offset szMin,0,REG_DWORD,offset task.min,4
invoke RegSetValueEx,hAppKeyN,offset szType,0,REG_DWORD,offset task.ttype,4
invoke CloseHandle,hAppKeyN
ret
_AddToRegistry   endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 开机自启动
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_AutoRun  proc	uses eax
            local @szFileName[256]:BYTE
invoke RegOpenKeyEx,HKEY_LOCAL_MACHINE,offset szSubRunKey,0,KEY_ALL_ACCESS,offset hRunKey
invoke GetModuleFileName,0,addr @szFileName,sizeof @szFileName
inc eax
invoke RegSetValueEx,hRunKey,offset szCaption,0,REG_SZ,addr @szFileName,eax
invoke CloseHandle,hRunKey
ret
_AutoRun   endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;从注册表删除指定任务
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_DelFromRegistry	proc	pszKeyName
	local @szBuf[100]:BYTE
	local @dwBuf

	invoke _QueryValue,pszKeyName
	test task.ttype,AWEEK
	.if ZERO?
		invoke wsprintf,addr @szBuf,offset szFmtKeyD,task.year,task.month,task.day,task.hour,task.min
		invoke RegOpenKeyEx,hKeyDate,addr @szBuf,NULL,KEY_ALL_ACCESS,offset hAppKeyN
		invoke RegDeleteKey,hAppKeyN,pszKeyName
		invoke RegDeleteKey,hKeyList,pszKeyName
	.else
		mov @dwBuf,0
		.while @dwBuf<=6
			mov eax,@dwBuf
			mov ecx,[eax*4+offset bTest]
			test task.ttype,ecx
			.if !ZERO?
				mov ebx,eax
				inc ebx
				invoke wsprintf,addr @szBuf,offset szFmtKeyZ,ebx,task.hour,task.min
				invoke RegOpenKeyEx,hKeyXingqi,addr @szBuf,NULL,KEY_ALL_ACCESS,offset hAppKeyN
				invoke RegDeleteKey,hAppKeyN,pszKeyName
			.endif
			inc @dwBuf
		.endw
		invoke RegDeleteKey,hKeyList,pszKeyName
	.endif
	invoke RegCloseKey,hAppKeyN
	ret
_DelFromRegistry	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;更新列表框
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_UpdateListBox   proc 
	local dwSize
	local lpcbData
	local dwIndex
	local szTskNameBuf[22]:BYTE
	mov dwIndex,0
	mov lpcbData,256
	invoke SendDlgItemMessage,hWinMain,IDC_LIST,LB_RESETCONTENT,0,0			;清除列表框原来的内容
   .while 1
   	mov dwSize,sizeof szTskNameBuf
   	invoke RegEnumKeyEx,hKeyList,dwIndex,addr szTskNameBuf,addr dwSize,0,0,0,0
   	.break .if eax==ERROR_NO_MORE_ITEMS
   	inc dwIndex
   	invoke SendDlgItemMessage,hWinMain,IDC_LIST,LB_ADDSTRING,0,addr szTskNameBuf
   .endw
	ret
_UpdateListBox   endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;列表控件子类化  新地址
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_ProcList	proc	uses ebx edi esi hWnd,wMsg,wParam,lParam
		mov eax,wMsg
		.if eax==WM_KEYDOWN
			mov eax,wParam
			.if eax==VK_DELETE
				invoke SendMessage,hWinMain,WM_COMMAND,IDC_DEL,0
			.elseif eax==VK_RETURN
				invoke SendMessage,hWinMain,WM_COMMAND,IDC_EDIT,0
			.endif
			invoke CallWindowProc,lpOldProc,hWnd,wMsg,wParam,lParam
		.else
			invoke CallWindowProc,lpOldProc,hWnd,wMsg,wParam,lParam
		.endif
		ret
_ProcList	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;提醒模块  窗口过程
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_ProcDlgShow	proc	uses ebx edi esi hWnd,wMsg,wParam,lParam
		local lpcbData
		local	stStartUp:STARTUPINFO
		local stProInfo:PROCESS_INFORMATION
		mov	eax,wMsg
		.if	eax == WM_INITDIALOG
			invoke RegOpenKeyEx,hKeyList,lParam,0,KEY_ALL_ACCESS,offset hAppKeyN
			invoke RegQueryValueEx,hAppKeyN,offset szType,0,NULL,offset task.ttype,addr lpcbData
			test task.ttype,NDLG
			.if ZERO?
				invoke EndDialog,hWnd,0
				jmp @f
			.endif
			invoke SetWindowPos,hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE				;设置顶层窗口
			invoke RegQueryValueEx,hAppKeyN,offset szName,0,NULL,offset task.name1,addr lpcbData
			invoke RegQueryValueEx,hAppKeyN,offset szText,0,NULL,offset task.text,addr lpcbData
			invoke SetDlgItemText,hWnd,IDC_NAMESHOW,offset task.name1
			invoke SetDlgItemText,hWnd,IDC_TXTSHOW,offset task.text
			invoke RegQueryValueEx,hAppKeyN,offset szPath,0,NULL,offset task.appath,addr lpcbData
			.if task.appath				;打开可执行文件
				invoke GetStartupInfo,addr stStartUp
				invoke CreateProcess,NULL,offset task.appath,NULL,NULL,NULL,NORMAL_PRIORITY_CLASS,NULL,NULL,addr stStartUp,addr stProInfo
			.endif
		@@:invoke RegCloseKey,hAppKeyN
		.elseif	eax==WM_COMMAND
			invoke	EndDialog,hWnd,0
		.elseif eax==WM_CLOSE
			invoke	EndDialog,hWnd,0
		.else	;其它消息
			mov	eax,FALSE
			ret
		.endif
		mov	eax,TRUE
		ret
_ProcDlgShow	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;线程函数
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
ThreadProc		proc	uses ebx esi edi	lParam
	invoke DialogBoxParam,hInstance,IDD_SHOW,NULL,offset _ProcDlgShow,lParam
	ret
ThreadProc		endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 定时器过程，守护
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_ProcTimerGuard	proc	uses ebx edi esi hWnd,uMsg,idEvent,dwTime
	local @second
	local @dwSize
	local @index
	local @buf[100]:BYTE
	local @systime:SYSTEMTIME
	invoke KillTimer,hWinMain,idTimer	;杀掉上个timer
	invoke GetLocalTime,addr @systime
	mov eax,60
	movzx ebx,@systime.wSecond
	sub eax,ebx
	mov @second,eax
	inc @second
	mov eax,1000
	mul @second
	invoke SetTimer,hWinMain,idTimer,eax,offset _ProcTimerGuard		;设置新的timer
	pushad
	movzx eax,@systime.wYear
	movzx ebx,@systime.wMonth
	movzx ecx,@systime.wDay
	movzx edx,@systime.wHour
	movzx esi,@systime.wMinute
	invoke wsprintf,addr @buf,offset szFmtKeyD,eax,ebx,ecx,edx,esi
	popad
	invoke RegOpenKeyEx,hKeyDate,addr @buf,0,KEY_ALL_ACCESS,offset hAppKeyN				;日期守护
	.if eax==ERROR_SUCCESS
		mov @index,0
		.while 1
			mov @dwSize,sizeof @dwSize
			invoke RegEnumKeyEx,hKeyList,@index,addr @buf,addr @dwSize,0,0,0,0
			.break .if eax==ERROR_NO_MORE_ITEMS
			invoke CreateThread,NULL,NULL,offset ThreadProc,addr @buf,0,NULL
			;invoke DialogBoxParam,hInstance,IDD_SHOW,NULL,offset _ProcDlgShow,addr @buf
			inc @index
		.endw
	.endif
	
	movzx eax,@systime.wDayOfWeek
	movzx ebx,@systime.wHour
	movzx ecx,@systime.wMinute
	invoke wsprintf,addr @buf,offset szFmtKeyZ,eax,ebx,ecx
	invoke RegOpenKeyEx,hKeyXingqi,addr @buf,0,KEY_ALL_ACCESS,offset hAppKeyN				;星期守护
	.if eax==ERROR_SUCCESS
		mov @index,0
		.while 1
			mov @dwSize,sizeof @dwSize
			invoke RegEnumKeyEx,hKeyList,@index,addr @buf,addr @dwSize,0,0,0,0
			.break .if eax==ERROR_NO_MORE_ITEMS
			invoke CreateThread,NULL,NULL,offset ThreadProc,addr @buf,0,NULL
			.if !eax
				invoke MessageBox,NULL,offset szError1,offset szCaption,MB_OK
			.endif
			;invoke DialogBoxParam,hInstance,IDD_SHOW,NULL,offset _ProcDlgShow,addr @buf
			inc @index
		.endw
	.endif
	invoke RegCloseKey,hAppKeyN
	ret
_ProcTimerGuard	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;“新建任务”窗口过程
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_ProcDlgNew	proc	uses ebx ecx edi esi hWnd,wMsg,wParam,lParam
		;****局部变量****
		local szCurSelName[22]:BYTE
		local @szBuf[200]:BYTE
		local @index
		local @systime:SYSTEMTIME
		local @stOF:OPENFILENAME
		local @dwSize
		local @sf			;SUCCESS or fail

		mov	eax,wMsg
		.if	eax == WM_CLOSE
			;***退出时代码***
			invoke EndDialog,hWnd,0
		.elseif	eax == WM_INITDIALOG
			;***初始化代码***
			.if @opration==EDIT
				;加载所选项
				invoke SetWindowText,hWnd,offset szEdit
				invoke SendDlgItemMessage,hWinMain,IDC_LIST,LB_GETCURSEL,0,0	;得到当前的所选项
				mov @index,eax
				invoke SendDlgItemMessage,hWinMain,IDC_LIST,LB_GETTEXT,@index,addr szCurSelName
				invoke _QueryValue,addr szCurSelName
				test task.ttype,AWEEK														;是否按周信息显示
				.if ZERO?																		;按日期
					invoke CheckRadioButton,hWnd,IDC_ADATE,IDC_AWEEK,IDC_ADATE;设置互斥的单选钮
					invoke SendMessage,hWnd,WM_COMMAND,IDC_ADATE,0
					invoke SetDlgItemInt,hWnd,IDC_YEAR,task.year,0
					invoke SetDlgItemInt,hWnd,IDC_MONTH,task.month,0
					invoke SetDlgItemInt,hWnd,IDC_DAY,task.day,0
				.else																				;按周
					invoke CheckRadioButton,hWnd,IDC_ADATE,IDC_AWEEK,IDC_AWEEK
					invoke SendMessage,hWnd,WM_COMMAND,IDC_ADATE,0
					mov @dwSize,0
					.while @dwSize<=6
						mov eax,@dwSize
						mov ecx,[eax*4+offset bTest]
						test task.ttype,ecx
						.if !ZERO?			;周测试通过
							mov edi,IDC_CHECK1
							add edi,eax
							invoke CheckDlgButton,hWnd,edi,BST_CHECKED
						.endif
						inc @dwSize
					.endw
				.endif
				test task.ttype,NDLG															;是否对话框提醒
				.if !ZERO?
					invoke CheckDlgButton,hWnd,IDC_ISDIALOG,BST_CHECKED
				.endif
				invoke SetDlgItemText,hWnd,IDC_NAME,offset task.name1
				invoke SetDlgItemText,hWnd,IDC_TskTEXT,offset task.text
				invoke SetDlgItemText,hWnd,IDC_FILEPATH,offset task.appath
				invoke SetDlgItemInt,hWnd,IDC_HOUR,task.hour,0
				invoke SetDlgItemInt,hWnd,IDC_MIN,task.min,0
			.else
				invoke CheckRadioButton,hWnd,IDC_ADATE,IDC_AWEEK,IDC_ADATE
				;加载当前时间
				invoke GetLocalTime,addr @systime
				movzx eax,@systime.wYear
				invoke SetDlgItemInt,hWnd,IDC_YEAR,eax,0
				movzx eax,@systime.wMonth
				invoke SetDlgItemInt,hWnd,IDC_MONTH,eax,0
				movzx eax,@systime.wDay
				invoke SetDlgItemInt,hWnd,IDC_DAY,eax,0
				movzx eax,@systime.wHour
				invoke SetDlgItemInt,hWnd,IDC_HOUR,eax,0
				movzx eax,@systime.wMinute
				invoke SetDlgItemInt,hWnd,IDC_MIN,eax,0
				
				invoke CheckDlgButton,hWnd,IDC_ISDIALOG,BST_CHECKED			;初始化复选框
				mov @dwSize,IDC_CHECK1
				.while @dwSize<=IDC_CHECK7
					invoke GetDlgItem,hWnd,@dwSize
					invoke EnableWindow,eax,0
					inc @dwSize
				.endw
			.endif
		.elseif	eax == WM_COMMAND
			;***子窗口命令***
			mov	eax,wParam
			.if	ax ==	IDCANCEL
				invoke EndDialog,hWnd,0
			.elseif	ax==IDC_SELFILE													;选择文件
				invoke RtlZeroMemory,addr @stOF,sizeof @stOF
				invoke RtlZeroMemory,addr @szBuf,sizeof @szBuf
				mov @stOF.lStructSize,sizeof @stOF
				push hWnd
				pop  @stOF.hwndOwner
				mov  @stOF.lpstrFilter,offset szFilter								;过滤文件
				lea  eax,@szBuf
				mov  @stOF.lpstrFile,eax												;文件路径名
				mov  @stOF.nMaxFile,MAX_PATH
				mov  @stOF.lpstrFileTitle,NULL										;文件名
				mov  @stOF.Flags,OFN_FILEMUSTEXIST  or OFN_PATHMUSTEXIST
				invoke GetOpenFileName,addr @stOF
				invoke SetDlgItemText,hWnd,IDC_FILEPATH,addr @szBuf
			.elseif	ax ==	IDOK
				.if @opration==EDIT;删除旧项
					invoke SendDlgItemMessage,hWinMain,IDC_LIST,LB_GETCURSEL,0,0
					mov ebx,eax
					invoke SendDlgItemMessage,hWinMain,IDC_LIST,LB_GETTEXT,ebx,addr szCurSelName
					invoke _DelFromRegistry,addr szCurSelName
				.endif
				;字符缓冲越界检查
				invoke GetDlgItemText,hWnd,IDC_NAME,offset task.name1,100
				.if eax>20
					invoke MessageBox,hWnd,offset szError1,offset szMsgCap,MB_OK
					jmp @f
				.elseif eax==0																			;空任务名检查
					invoke MessageBox,hWnd,offset szError4,offset szMsgCap,MB_OK
					jmp @f
				.endif
				invoke GetDlgItemText,hWnd,IDC_TskTEXT,offset task.text,1024
				.if eax>200
					invoke MessageBox,hWnd,offset szError2,offset szMsgCap,MB_OK
					jmp @f
				.endif
				;任务同名检查
				mov @index,0
				 .while 1
					mov @dwSize,sizeof szCurSelName
					invoke RegEnumKeyEx,hKeyList,@index,addr szCurSelName,addr @dwSize,0,0,0,0
					.break .if eax==ERROR_NO_MORE_ITEMS
					invoke lstrcmp,addr szCurSelName,offset task.name1
					.if eax==0
						invoke MessageBox,hWnd,offset szSameName,offset szMsgCap,MB_OK
						jmp @f
					.endif
					inc @index
				.endw
				;执行程序路径
				invoke GetDlgItemText,hWnd,IDC_FILEPATH,offset task.appath,200
				mov task.ttype,0
				;时间项  时  分
				invoke GetDlgItemInt,hWnd,IDC_HOUR,addr @sf,0
				mov task.hour,eax
				invoke GetDlgItemInt,hWnd,IDC_MIN,addr @sf,0
				mov task.min,eax
				;按日期
				invoke	IsDlgButtonChecked,hWnd,IDC_ADATE
				.if eax==BST_CHECKED
					or task.ttype,ADATE
					;填充Task结构
					invoke GetDlgItemInt,hWnd,IDC_YEAR,addr @sf,0
					mov task.year,eax
					invoke GetDlgItemInt,hWnd,IDC_MONTH,addr @sf,0
					mov task.month,eax
					invoke GetDlgItemInt,hWnd,IDC_DAY,addr @sf,0
					mov task.day,eax
					;建立索引项
					invoke wsprintf,addr @szBuf,offset szFmtKeyD,task.year,task.month,task.day,task.hour,task.min
					invoke RegCreateKeyEx,hKeyDate,addr @szBuf,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,offset hAppKeyN,0
					invoke RegCreateKeyEx,hAppKeyN,offset task.name1,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,offset hAppKeyN,0
					invoke RegCloseKey,hAppKeyN
				.endif
				;按星期
				invoke	IsDlgButtonChecked,hWnd,IDC_AWEEK
				.if eax==BST_CHECKED
					or task.ttype,AWEEK
					;填充周  信息
					mov @dwSize,1051
					.while @dwSize<=1057
						invoke	IsDlgButtonChecked,hWnd,@dwSize
						.if eax==BST_CHECKED
							mov eax,@dwSize
							sub eax,1050
							dec eax
							mov ecx,[eax*4+offset bTest]
							or task.ttype,ecx
							mov ebx,eax
							inc ebx
							;建立索引项
							invoke wsprintf,addr @szBuf,offset szFmtKeyZ,ebx,task.hour,task.min
							invoke RegCreateKeyEx,hKeyXingqi,addr @szBuf,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,offset hAppKeyN,0
						.endif
						inc @dwSize
					.endw
					invoke RegCloseKey,hAppKeyN
				.endif
				;是否用对话框提示
				invoke	IsDlgButtonChecked,hWnd,IDC_ISDIALOG
				.if eax==BST_CHECKED
					or task.ttype,NDLG
				.endif
				;添加到list下
				invoke _AddToRegistry
				invoke EndDialog,hWnd,0
				;选择日期星期时的效果
			.elseif ax==IDC_ADATE
				mov @dwSize,IDC_YEAR
				.while @dwSize<=IDC_DAY
					invoke GetDlgItem,hWnd,@dwSize
					invoke EnableWindow,eax,1
					inc @dwSize
				.endw
				
				mov @dwSize,IDC_CHECK1
				.while @dwSize<=IDC_CHECK7
					invoke GetDlgItem,hWnd,@dwSize
					invoke EnableWindow,eax,0
					inc @dwSize
				.endw
			.elseif ax==IDC_AWEEK
				mov @dwSize,IDC_CHECK1
				.while @dwSize<=IDC_CHECK7
					invoke GetDlgItem,hWnd,@dwSize
					invoke EnableWindow,eax,1
					inc @dwSize
				.endw
				
				mov @dwSize,IDC_YEAR
				.while @dwSize<=IDC_DAY
					invoke GetDlgItem,hWnd,@dwSize
					invoke EnableWindow,eax,0
					inc @dwSize
				.endw
			.endif

		.else	;其它消息
			mov	eax,FALSE
			ret
		.endif
		@@:mov	eax,TRUE
		ret

_ProcDlgNew	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 主窗口过程
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
_ProcDlgMain	proc	uses ebx edi esi hWnd,wMsg,wParam,lParam
		;****局部变量****
		local dwSize
		local szCurSelName[22]:BYTE
		local @szBuf[200]:BYTE
		local @szBuf2[100]:BYTE
		local hIcon
		local ntico:NOTIFYICONDATA
		mov	eax,wMsg
		.if	eax == WM_CLOSE
			;***退出时代码***
			invoke ShowWindow,hWnd,SW_HIDE
			;invoke Shell_NotifyIcon,NIM_DELETE,addr ntico
			;invoke EndDialog,hWnd,0
		.elseif	eax == WM_INITDIALOG
			;***初始化代码***
			invoke SetWindowText,hWnd,offset szCaption
			invoke LoadIcon,hInstance,IDI_ICON1
			mov hIcon,eax
			invoke SendMessage,hWnd,WM_SETICON,ICON_BIG,eax
			
			;注册热键
			invoke RegisterHotKey,hWnd,0,MOD_ALT,VK_P
			
			;右下角图标
			mov ntico.cbSize,sizeof NOTIFYICONDATA
			push hWnd
			pop ntico.hwnd
			mov ntico.uID,0
			mov ntico.uFlags,NIF_MESSAGE OR NIF_ICON OR NIF_TIP
			mov ntico.uCallbackMessage,WM_ICON
			push hIcon
			pop ntico.hIcon
			mov ebx,offset szIcon
			lea eax,ntico.szTip
			mov esi,eax
			mov eax,0
			.while eax<=6
			push [ebx]
			pop  [esi]
			add ebx,4
			add esi,4
			inc eax
			.endw
			push 0
			pop [esi]
			invoke Shell_NotifyIcon,NIM_ADD,addr ntico
			
			push hWnd
			pop hWinMain
			invoke GetDlgItem,hWnd,IDC_LIST
			invoke SetWindowLong,eax,GWL_WNDPROC,offset _ProcList
			mov lpOldProc,eax
			invoke RegCreateKeyEx,HKEY_LOCAL_MACHINE,\	;打开应用程序使用的hKey：hAppKey，供程序使用
										offset szKeyList,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,offset hKeyList,0
			invoke RegCreateKeyEx,HKEY_LOCAL_MACHINE,\	;打开应用程序使用的hKey：hAppKey，供程序使用
										offset szKeyDate,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,offset hKeyDate,0
			invoke RegCreateKeyEx,HKEY_LOCAL_MACHINE,\	;打开应用程序使用的hKey：hAppKey，供程序使用
										offset szKeyXingqi,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,offset hKeyXingqi,0
			invoke _UpdateListBox
			;设置定时器守护任务
			invoke SetTimer,hWinMain,idTimer,0,offset _ProcTimerGuard
			invoke GetDlgItem,hWnd,IDC_NEW
			invoke SetFocus,eax
		.elseif	eax == WM_COMMAND
			;***子窗口命令***
			mov	eax,wParam
			.if	ax==IDC_NEW	;其它子窗口命令
				mov @opration,NEW
				invoke DialogBoxParam,hInstance,IDD_NEW,hWnd,offset _ProcDlgNew,0
				invoke _UpdateListBox
			.elseif	ax==IDC_EDIT
				mov @opration,EDIT
				invoke DialogBoxParam,hInstance,IDD_NEW,hWnd,offset _ProcDlgNew,0
				invoke _UpdateListBox
			.elseif	ax==IDC_DEL
				invoke SendDlgItemMessage,hWnd,IDC_LIST,LB_GETCURSEL,0,0
				mov ebx,eax
				invoke SendDlgItemMessage,hWinMain,IDC_LIST,LB_GETTEXT,ebx,addr szCurSelName
				invoke _DelFromRegistry,addr szCurSelName
				invoke _UpdateListBox
			.elseif	ax==IDC_ABOUT
				invoke MessageBox,hWnd,offset szAbout,offset szAboutCap,MB_OK
			.elseif	ax==IDC_LIST
				shr	eax,16
				.if ax==LBN_SELCHANGE
					invoke SendDlgItemMessage,hWnd,IDC_LIST,LB_GETCURSEL,0,0
					mov ebx,eax
					invoke SendDlgItemMessage,hWnd,IDC_LIST,LB_GETTEXT,ebx,addr szCurSelName
					invoke _QueryValue,addr szCurSelName
					;显示任务名称和内容
					invoke wsprintf,addr @szBuf,offset szFmt,offset task.name1,offset task.text,offset task.appath
					;按周或日期显示任务详情
					test task.ttype,AWEEK
					.if ZERO?;adata
						invoke wsprintf,addr @szBuf2,offset szFmtData,task.year,task.month,task.day,task.hour,task.min
						invoke lstrcat,addr @szBuf,addr @szBuf2
					.else		;aweek
						invoke wsprintf,addr @szBuf2,offset szFmtTime,task.hour,task.min
						invoke lstrcat,addr @szBuf,addr @szBuf2
						mov dwSize,0
						.while dwSize<=6
							mov eax,dwSize
							mov ecx,[eax*4+offset bTest]
							test task.ttype,ecx
							.if !ZERO?			;周测试通过
								mov eax,dwSize
								shl eax,3		;乘8
								mov edi,offset szFmtWeek
								add edi,eax
								invoke lstrcat,addr @szBuf,edi
							.endif
							inc dwSize
						.endw
					.endif
					invoke GetDlgItem,hWnd,IDC_TEXT
					mov ebx,eax
					invoke SetWindowText,ebx,addr @szBuf
				.endif
			.endif
		.elseif eax==WM_ICON
			.if wParam==0
				.if lParam==WM_LBUTTONDBLCLK
					invoke ShowWindow,hWinMain,SW_RESTORE
					invoke GetDlgItem,hWnd,IDC_NEW
					invoke SetFocus,eax
				.endif
			.endif
		.elseif eax==WM_HOTKEY
			.if bShow
				invoke ShowWindow,hWinMain,SW_RESTORE
				invoke GetDlgItem,hWnd,IDC_NEW
				invoke SetFocus,eax
			.else
				invoke ShowWindow,hWinMain,SW_HIDE
			.endif
			not bShow
		.else	;其它消息
			mov	eax,FALSE
			ret
		.endif
	@@:mov	eax,TRUE
		ret

_ProcDlgMain	endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
start:
		invoke FindWindow,NULL,offset szCaption
		.if	eax
			invoke ShowWindow,eax,SW_RESTORE
			invoke GetDlgItem,hWinMain,IDC_NEW
			invoke SetFocus,eax
			invoke	ExitProcess,NULL
		.endif
		invoke	GetModuleHandle,NULL
		mov	hInstance,eax
		invoke _AutoRun
		invoke	DialogBoxParam,hInstance,IDD_MAIN,NULL,offset _ProcDlgMain,NULL
		invoke	ExitProcess,NULL
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		end	start

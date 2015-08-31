#include "ntddk.h"
#include "winstring.h"

#define		IOCTL_INIT   (ULONG) CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define		IOCTL_GETAWINDOW	(ULONG) CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define		IOCTL_DECIDEOK		(ULONG) CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define		IOCTL_DECIDENO		(ULONG) CTL_CODE(FILE_DEVICE_UNKNOWN, 0x804, METHOD_BUFFERED, FILE_ANY_ACCESS)
//*************************************************************************
//相关结构
//*************************************************************************
//窗口信息
typedef struct window_info
{
	TCHAR window_name[64];
	TCHAR class_name[64];
	int height;
	int width;
}WINDOW_INFO,*PWINDOW_INFO;
WINDOW_INFO wndinfo;
//初始化信息
struct INITINFO
{
	PWINDOW_INFO p;
}init_info;

typedef struct _SERVICE_DESCRIPTOR_TABLE
{
	PVOID    ServiceTableBase;
	PULONG   ServiceCounterTableBase;
	ULONG    NumberOfServices;
	PULONG    ParamTableBase;
}SERVICE_DESCRIPTOR_TABLE, *PSERVICE_DESCRIPTOR_TABLE;

int (*old_fnCreateWindow)(int dwExStyle,PWINUNICODESTRING UnsafeClassName,PWINUNICODESTRING UnsafeWindowName,int dwUnknown1,int dwStyle,LONG x, LONG y,LONG nWidth,LONG nHeight,int hWndParent,int hMenu,int hInstance,int lpParam,int dwShowMode,int dwUnknown2);

//*************************************************************************
//全局变量
//*************************************************************************
ULONG *lpfnBaseInTable,fnOldAddress;
int bDecide;
int fnIndex;
KEVENT event_read,event_decide;
KEVENT event_wnd;//用来同步不同线程之间的创建窗口
				//不同自旋锁的原因是 事件能够自主的交出cpu时间
int bPass;
int window_handle;
KSPIN_LOCK kl;
int pShadow;
PSERVICE_DESCRIPTOR_TABLE KeServiceDescriptorTableShadow;
__declspec(dllimport)  SERVICE_DESCRIPTOR_TABLE KeServiceDescriptorTable; 
__declspec(dllimport) _stdcall KeAddSystemServiceTable(PVOID, PVOID, PVOID, PVOID, PVOID);



//HOOK函数声明
int _stdcall my_NtUserCreateWindowEx(int dwExStyle,PWINUNICODESTRING ClassName,PWINUNICODESTRING WindowName,int dwUnknown1,int dwStyle,LONG x, LONG y,LONG nWidth, LONG nHeight,int hWndParent,int hMenu,int hInstance,int lpParam,int dwShowMode,int dwUnknown2)
 { 
	//若进入等待，则说明有其它线程正在调用本函数
	KeWaitForSingleObject(&event_wnd,Executive,KernelMode,FALSE,NULL);
	 //KeAcquireSpinLock(&kl,&irql);

	KdPrint(("There is a window being created!\n"));
	if(hWndParent==NULL)
	{
		_try
		{
		//传递窗口信息
		KdPrint(("WindowName:%s\n",WindowName->buf));
		//_asm int 3
		CopyWinUnicodeString(wndinfo.window_name,WindowName->buf,64);
		CopyWinUnicodeString(wndinfo.class_name,ClassName->buf,64);
		wndinfo.height=nHeight;
		wndinfo.width=nWidth;
		}
		_except(STATUS_ACCESS_VIOLATION)
		{
			window_handle=old_fnCreateWindow(dwExStyle,ClassName,WindowName,dwUnknown1,dwStyle,x,y,nWidth,nHeight,hWndParent,hMenu,hInstance,lpParam,dwShowMode,dwUnknown2); 
			goto exit;
		}
		//通知判断程序有窗口创建
		KeSetEvent(&event_read,0,1);
		//等待判断
		KeWaitForSingleObject(&event_decide,Executive,KernelMode,FALSE,NULL);
		if(bDecide)
		{
			window_handle=old_fnCreateWindow(dwExStyle,ClassName,WindowName,dwUnknown1,dwStyle,x,y,nWidth,nHeight,hWndParent,hMenu,hInstance,lpParam,dwShowMode,dwUnknown2); 
		}
		else
		{
			window_handle=0;
		}
		KdPrint(("The window handle is %x\n",window_handle));
	}
exit:	
	KeSetEvent(&event_wnd,0,1);//离开时不要忘了设置事件有信号
	//KeReleaseSpinLock(&kl,&irql);
	return window_handle;
 } 

//*************************************************************************
//实现HOOK相关函数
//*************************************************************************
//得到ShadowSSDT的地址(搜过特征码法)
unsigned int getAddressOfShadowTable()
{
    unsigned int i;
    unsigned char *p;
    unsigned int intatbyte;

    p = (unsigned char*) KeAddSystemServiceTable;

    for(i = 0; i < 4096; i++, p++)
    {
        __try
        {
            intatbyte = *(unsigned int*)p;
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return 0;
        }

        if(MmIsAddressValid((PVOID)intatbyte))
        {
            if(memcmp((PVOID)intatbyte, &KeServiceDescriptorTable, 16) == 0)
            {
                if((PVOID)intatbyte == &KeServiceDescriptorTable)
                {
                    continue;
                }

                return intatbyte;
            }
        }
    }

    return 0;
}
//（PETHREAD->ServiceTable字段法）
ULONG GetShadowAddrFromEthread()
{
	int i;
	PETHREAD pEthread=PsGetCurrentThread();
	KdPrint(("ETHREAD结构地址：%x\n",pEthread));
	for(i=0;i<256*4;i++)
	{
		if(*(pEthread+i)==KeServiceDescriptorTableShadow)
		{
			KdPrint(("ServiceTable字段的偏移：%d\n",i));
			return *(pEthread+i);
		}
	}
	//_asm int 3
	return 5555;//0xdc为ServiceTable字段的偏移（ETHREAD结构不透明）
}

ULONG getShadowTable()
{
    KeServiceDescriptorTableShadow =getAddressOfShadowTable();
	
	KdPrint(("搜索特征码：%x\n",KeServiceDescriptorTableShadow));
	
	KdPrint(("ServiceTable字段：%x\n",GetShadowAddrFromEthread()));

    if(KeServiceDescriptorTableShadow == NULL)
    {
        DbgPrint("Couldnt find shadowtable!\n");
        
        return FALSE;
    }
    else
    {
        DbgPrint("Shadowtable has been found!\n");
        
        DbgPrint("Shadowtable entries: %d\n", KeServiceDescriptorTableShadow[1].NumberOfServices);
        return TRUE;
    }
} 

//根据操作系统来确定具体函数的服务号 
VOID InitCallNumber()
{
	ULONG majorVersion, minorVersion;
	PsGetVersion( &majorVersion, &minorVersion, NULL, NULL );
    if ( majorVersion == 5 && minorVersion == 2 )
    {
	  DbgPrint("comint32: Running on Windows 2003\n");

	}
	else if ( majorVersion == 5 && minorVersion == 1 )
	{
	  DbgPrint("comint32: Running on Windows XP\n");
      fnIndex=343;
	}
	else if ( majorVersion == 5 && minorVersion == 0 )
	{
	  DbgPrint("comint32: Running on Windows 2000\n");
	}
}

// xp下NtUserCreateWindowEx索引号为343
//param1：函数在shadow中的索引号  param2：新函数的地址  
//返回值：旧函数的地址
ULONG Hook_ShadowSSDT(ULONG fnID,PVOID fnNew)
{
	PMDL pShadow;
	int size;
	int base;
	int i=0;
	
	_asm
	  {
		CLI                    //dissable interrupt
		MOV    EAX, CR0        //move CR0 register into EAX
		AND EAX, NOT 10000H //disable WP bit 
		MOV    CR0, EAX        //write register back
	  }
	
	getShadowTable();
	size=KeServiceDescriptorTableShadow[1].NumberOfServices*4;
	base=KeServiceDescriptorTableShadow[1].ServiceTableBase;
	
	KdPrint(("ShadowSSDT基址为：%x\n",base));

	lpfnBaseInTable = (ULONG *)(base+343*4);
	fnOldAddress=(ULONG)*lpfnBaseInTable;
	
	KdPrint(("NuUserCreateWindowEx原函数地址：%x\n",fnOldAddress));
	KdPrint(("NuUserCreateWindowEx新函数地址：%x\n",fnNew));
	
	InterlockedExchange(lpfnBaseInTable,fnNew);
	
	_asm 
	  {
		MOV    EAX, CR0        //move CR0 register into EAX
		OR     EAX, 10000H        //enable WP bit     
		MOV    CR0, EAX        //write register back        
		STI                    //enable interrupt
	  }
	
	KdPrint(("Hook 成功\n"));
	return fnOldAddress;
}

//*************************************************************************
//驱动框架
//*************************************************************************
VOID DriverUnload(PDRIVER_OBJECT driver)
{
	DbgPrint("Driver Unload!\n");
	//恢复hook
	InterlockedExchange(lpfnBaseInTable,fnOldAddress);
}

//默认的IRP请求处理函数
//
NTSTATUS fnIrpDefault(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	PIO_STACK_LOCATION pSP=IoGetCurrentIrpStackLocation(Irp);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	if(DeviceObject->Flags & DO_DIRECT_IO)
	{
		KdPrint(("Flags:DO_DIRECT_IO\n"));
	}
	if(DeviceObject->Flags & DO_BUFFERED_IO)
	{
		KdPrint(("Flags:DO_BUFFERED_IO\n"));
	}
	switch (pSP->MajorFunction)
	{
	case  IRP_MJ_READ:
		KdPrint(("MJ:IRP_MJ_READ\n"));
		break;
	case IRP_MJ_CREATE:
		KdPrint(("MJ:IRP_MJ_CREATE\n"));
		break;	
	}
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return Irp->IoStatus.Status;
}

//IRP写请求处理例程
NTSTATUS fnIrpWrite(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	NTSTATUS status = STATUS_SUCCESS;
	PIO_STACK_LOCATION pSP = IoGetCurrentIrpStackLocation(Irp);
	PUCHAR pBuffer = NULL;
	UCHAR  port=0x64;
	UCHAR code;
	
	KdPrint(("写请求\n"));

	pBuffer = Irp->AssociatedIrp.SystemBuffer;
	KdPrint(("IRP输入缓冲区: %x\n",pBuffer));

	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = 1;

	IoCompleteRequest(Irp,IO_NO_INCREMENT);

	return status;
}

NTSTATUS fnIrpDeviceControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	NTSTATUS status = STATUS_SUCCESS;
	PIO_STACK_LOCATION pSP = IoGetCurrentIrpStackLocation(Irp);
	PUCHAR pBuffer = NULL;
	ULONG code=pSP->Parameters.DeviceIoControl.IoControlCode;
	pBuffer = Irp->AssociatedIrp.SystemBuffer;
	KdPrint(("控制请求\n"));
	KdPrint(("IRP输入缓冲区: %x\n",pBuffer));

	switch(code)
	{
		case IOCTL_INIT:
		KeSetEvent(&event_wnd,0,1);
		//Hook Shadow
		KdPrint(("Hook Shadow ...\n",pBuffer));
		old_fnCreateWindow=Hook_ShadowSSDT(fnIndex,my_NtUserCreateWindowEx);
		RtlCopyMemory(&init_info.p,pBuffer,4);

		//KdPrint(("传入的缓冲区地址：%x\n",init_info.p));
		//_asm int 3
		break;
		case IOCTL_GETAWINDOW:
		KeWaitForSingleObject(&event_read,UserRequest,KernelMode,FALSE,NULL);
		//_asm  int 3
		RtlCopyMemory(init_info.p,&wndinfo,sizeof(wndinfo));
		KdPrint(("Got a window!"));
		break;
		case IOCTL_DECIDEOK://允许创建
		bDecide=1;
		KeSetEvent(&event_decide,0,1);
		break;
		case IOCTL_DECIDENO://不允许创建
		bDecide=0;
		KeSetEvent(&event_decide,0,1);
		break;
	}

	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = 1;

	IoCompleteRequest(Irp,IO_NO_INCREMENT);

	return status;
}

//IRP读请求处理例程
NTSTATUS fnIrpRead(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	NTSTATUS status = STATUS_SUCCESS;
	PIO_STACK_LOCATION pSP = IoGetCurrentIrpStackLocation(Irp);
	PUCHAR pBuffer = NULL;
	UCHAR code;


	KdPrint(("读请求\n"));

	pBuffer = Irp->AssociatedIrp.SystemBuffer;
	KdPrint(("IRP输出缓冲区: %x\n",pBuffer));
	
	KeWaitForSingleObject(&event_read,Executive,KernelMode,0,0);
	KdPrint(("有窗口创建，句柄	：%x\n",window_handle));

	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = 1;

	IoCompleteRequest(Irp,IO_NO_INCREMENT);

	return status;
}

//驱动入口点
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
	PDEVICE_OBJECT DeviceObject;
	UNICODE_STRING DeviceName,LinkName;
	NTSTATUS status;
	int i;
	
	//判读操作系统版本
	InitCallNumber();
	
	//初始化事件对象
	KeInitializeEvent(&event_read,SynchronizationEvent,0);
	KeInitializeEvent(&event_decide,SynchronizationEvent,0);
	KeInitializeEvent(&event_wnd,SynchronizationEvent,0);

	//KeInitializeSpinLock(&kl);

	//初始化字符串
	RtlInitUnicodeString(&DeviceName,L"\\Device\\FilterWindow");
	RtlInitUnicodeString(&LinkName,L"\\??\\FilterWindow");
	
	//指定IRP处理例程
	for(i=0;i<=IRP_MJ_MAXIMUM_FUNCTION;i++)
		DriverObject->MajorFunction[i]=fnIrpDefault;
	DriverObject->MajorFunction[IRP_MJ_WRITE]=fnIrpWrite;
	DriverObject->MajorFunction[IRP_MJ_READ]=fnIrpRead;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]=fnIrpDeviceControl;
	//创建设备
	status=IoCreateDevice(DriverObject,0, &DeviceName,FILE_DEVICE_UNKNOWN,0,0,&DeviceObject);

	if(!NT_SUCCESS(status))
	{
		DbgPrint("设备创建失败\n");
		return status;
	}
	
	//设置设备的读写方式
	DeviceObject->Flags |=DO_BUFFERED_IO;
	//创建符号链接
	status=IoCreateSymbolicLink(&LinkName,&DeviceName);

	DeviceObject->Flags&=~DO_DEVICE_INITIALIZING;//初始化完成标记

	if(!NT_SUCCESS(status))
	{
		DbgPrint("符号连接创建失败\n");
		return status;
	}
	DbgPrint("符号连接创建成功\n");
	
	return STATUS_SUCCESS;
}







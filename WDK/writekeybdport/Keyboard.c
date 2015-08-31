#include "ntddk.h"
//驱动卸载例程
VOID DriverUnload(PDRIVER_OBJECT driver)
{
	DbgPrint("键盘端口驱动卸载ing\n");
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
		KdPrint(("Flags:DO_DIRECT_IO"));
	}
	if(DeviceObject->Flags & DO_BUFFERED_IO)
	{
		KdPrint(("Flags:DO_BUFFERED_IO"));
	}
	switch (pSP->MajorFunction)
	{
	case  IRP_MJ_READ:
		KdPrint(("MJ:IRP_MJ_READ"));
		break;
	case IRP_MJ_CREATE:
		KdPrint(("MJ:IRP_MJ_CREATE"));
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


	KdPrint(("收到irp write\n"));

		pBuffer = Irp->AssociatedIrp.SystemBuffer;
		KdPrint(("IRP输入缓冲区: %x\n",pBuffer));
		code=(UCHAR)(*pBuffer);

		KdPrint(("IRP_MJ_WRITE Write Len: %d\n",pSP->Parameters.Write.Length));
		//向键盘端口写入数据
		//WRITE_PORT_UCHAR(&port,(UCHAR)(*pBuffer));
		_asm
	{
			push eax
			mov al,0xd2
			out 0x64,al
			mov al,code
			out 0x60,al
			pop eax
		}

	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = 1;

	IoCompleteRequest(Irp,IO_NO_INCREMENT);

	return status;
}

//驱动入口函数
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
	UNICODE_STRING DeviceName,LinkName;
	PDEVICE_OBJECT DeviceObject=NULL;
	NTSTATUS status;		int i;

	DriverObject->DriverUnload=DriverUnload;

	//输出调试信息
	DbgPrint("键盘端口驱动安装ing\n");
	//初始化字符串
	RtlInitUnicodeString(&DeviceName,L"\\Device\\wriKeyPort");
	RtlInitUnicodeString(&LinkName,L"\\??\\wriKeyPort");
	//指定IRP处理例程
	for(i=0;i<=IRP_MJ_MAXIMUM_FUNCTION;i++)
		DriverObject->MajorFunction[i]=fnIrpDefault;
	DriverObject->MajorFunction[IRP_MJ_WRITE]=fnIrpWrite;
	//创建设备
	status=IoCreateDevice(DriverObject,4, &DeviceName,FILE_DEVICE_UNKNOWN,0,0,&DeviceObject);

	if(!NT_SUCCESS(status))
	{
		DbgPrint("设备创建失败\n");
		return status;
	}

	if(!DeviceObject)return STATUS_UNEXPECTED_IO_ERROR;

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
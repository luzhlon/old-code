//
//×¢ÊÍ
//
#include "ntddk.h"

VOID DriverUnload(PDRIVER_OBJECT driver)
{
	DbgPrint("Driver Unload!\n");
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
	return STATUS_SUCCESS;
}
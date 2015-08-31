//
//×¢ÊÍ
//
#include "ntddk.h"

typedef 
struct _SERVICE_DESCRIPTOR_TABLE
{
	PVOID    ServiceTableBase;
	PULONG   ServiceCounterTableBase;
	ULONG    NumberOfServices;
	PUCHAR    ParamTableBase;
}
SERVICE_DESCRIPTOR_TABLE, *PSERVICE_DESCRIPTOR_TABLE;

__declspec(dllimport) SERVICE_DESCRIPTOR_TABLE KeServiceDescriptorTable;

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
	KdPrint(("SSDT»ùÖ·£º%u",KeServiceDescriptorTable));
	return STATUS_SUCCESS;
}
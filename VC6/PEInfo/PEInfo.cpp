// PEInfo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>

int fgetsize(char* path)
{
	FILE* f = fopen(path, "r");
	if(0 == f)
	{
		return -1;
	}
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	fclose(f);
	return size;
}

void PrintIndent(int count)
{
	if(count < 0)
	{
		return;
	}
	for(; count; count--)
	{
		printf("   ");
	}
}

int main(int argc, char* argv[])
{

	_asm
	{
		call n
	n:  pop eax
	}

	if(argc < 2)
	{
		printf("No arguments.");
		getchar();
		return 0;
	}

	PIMAGE_DOS_HEADER pDosHeader;				//DOS文件头
	PIMAGE_NT_HEADERS pNtHeaders;				//PE(NT)文件头
	int nIndent = 0;							//缩进数

	int fsize = fgetsize(argv[1]);				//获得文件大小
	if(-1 == fsize)
	{
		printf("fgetsize failue.");
		getchar();
		return 0;
	}

	FILE *file = fopen(argv[1], "r");		//打开文件
	if(0 == file)
	{
		printf("File open failue.");
		getchar();
		return 0;
	}

	PBYTE base = (PBYTE)malloc(fsize);				//分配缓冲区存放文件
	if(0 == base)
	{
		printf("malloc failue.");
		getchar();
		return 0;
	}

	if(!fread(base, 1, fsize, file))			//将文件读入缓冲区
	{
		printf("fread failue.");
		getchar();
		return 0;
	}

	//定位PE文件头
	pDosHeader = (PIMAGE_DOS_HEADER)base;
	pNtHeaders = (PIMAGE_NT_HEADERS)(base + pDosHeader->e_lfanew - 1);

	printf("Nt_Headers offset: 0x%x\n", pDosHeader->e_lfanew);
	printf("IMAGE_NT_HEADERS:\n");
	printf("   Signature: %s\n", &pNtHeaders->Signature);
	printf("   FileHeader(IMAGE_FILE_HEADER):\n");
	printf("      Machine:             0x%x		//运行平台\n", pNtHeaders->FileHeader.Machine);
	printf("      NumberOfSections:    0x%x		//节区（表）数\n", pNtHeaders->FileHeader.NumberOfSections);
	printf("      SizeOfOptionalHeader:0x%x		//\n", pNtHeaders->FileHeader.SizeOfOptionalHeader);
	printf("      Characteristics:     0x%x		//属性\n", pNtHeaders->FileHeader.Characteristics);
	printf("   OptionalHeader(IMAGE_OPTIONAL_HEADER32):\n");
	printf("      AddressOfEntryPoint: 0x%x		//程序执行入口RVA\n", pNtHeaders->OptionalHeader.AddressOfEntryPoint);
	printf("      ImageBase:           0x%x		//建议装载地址\n", pNtHeaders->OptionalHeader.ImageBase);
	//printf("%x\n", GetModuleHandle(0));
	printf("      FileAlignment:       0x%x		//文件中节的对齐粒度\n", pNtHeaders->OptionalHeader.FileAlignment);
	printf("      SectionAlignment:    0x%x		//内存中节的对齐粒度\n", pNtHeaders->OptionalHeader.SectionAlignment);
	printf("      Subsystem:           0x%x		//界面子系统\n", pNtHeaders->OptionalHeader.Subsystem);
	printf("");
	printf("");
	printf("");
	printf("");
	printf("");
	printf("");
	printf("");
	printf("");
	printf("");
	printf("");
	printf("");
	printf("");

	//HMODULE h = LoadLibrary("kernel32.dll");
	//printf("%x", GetProcAddress(h, "LoadLibraryW"));

	getchar();
	return 0;
}

#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

#include <Windows.h>
//#include <wx/debug.h>
#include "stdio.h"
#include "assert.h"
#include "dbgeng.h"

#ifdef _DEBUG
#define DbgPrint(s) OutputDebugString(s)
#else
#define DbgPrint(s)
#endif
//#define ASSERT(b, s) wxASSERT_MSG(b,s)
#define ASSERT(b, s) assert(b)

class Debugger
{
public:
	Debugger(PSTR cmd = NULL, ULONG flags = DEBUG_CREATE_PROCESS_NO_DEBUG_HEAP|DEBUG_PROCESS,
		ULONG64 srv = NULL);
	Debugger(ULONG id = NULL, ULONG flags = DEBUG_CREATE_PROCESS_NO_DEBUG_HEAP|DEBUG_PROCESS,
		ULONG64 srv = NULL);
	~Debugger();

	int CreateProcess(PSTR cmd, ULONG flags = DEBUG_CREATE_PROCESS_NO_DEBUG_HEAP|DEBUG_PROCESS,
		ULONG64 srv = NULL);
	int AttachProcess(ULONG id, ULONG flags = DEBUG_CREATE_PROCESS_NO_DEBUG_HEAP|DEBUG_PROCESS,
		ULONG64 srv = NULL);
	int SetInputCallback(IDebugInputCallbacks* input)
	{
		ASSERT(m_Client, "DebugClient was not created!");
		return m_Client->SetInputCallbacks(input) == S_OK;
	}
	int SetOutputCallback(IDebugOutputCallbacks* output)
	{
		ASSERT(m_Client, "DebugClient was not created!");
		return m_Client->SetOutputCallbacks(output) == S_OK;
	}
	int SetEventCallback(IDebugEventCallbacks* event)
	{
		ASSERT(m_Client, "DebugClient was not created!");
		return m_Client->SetEventCallbacks(event) == S_OK;
	}

	HRESULT WaitForEvent(ULONG Flags = DEBUG_WAIT_DEFAULT, ULONG Timeout = INFINITE)
	{
		//ASSERT(m_Client, "DebugClient was not created!");
		//ASSERT(m_Control, "DebugControl was not created!");
		return m_Control->WaitForEvent(Flags, Timeout);
	}

	class InputCallback : public IDebugInputCallbacks
	{
	public:
		virtual ULONG _stdcall AddRef() { return 0; }
		virtual ULONG _stdcall Release() { return 1; }
		virtual HRESULT _stdcall QueryInterface(REFIID id, void **ppvObj)
		{
			*ppvObj = this;
			return NOERROR;
		}

		virtual HRESULT _stdcall StartInput(IN ULONG BuffSize)
		{
			//printf("[StartInput] BuffSize:%u\n", BuffSize);
			return 0;
		}

		virtual HRESULT _stdcall EndInput()
		{
			//printf("[EndInput]\n");
			return 0;
		}
	};//Class Debugger::InputCallbacks END

	class OutputCallback : public IDebugOutputCallbacks
	{
	public:
		virtual ULONG _stdcall AddRef()
		{
			return 0;
		}
		virtual ULONG _stdcall Release()
		{
			return 1;
		}
		virtual HRESULT _stdcall QueryInterface(REFIID id, void **ppvObj)
		{
			*ppvObj = NULL;

			if (IsEqualIID(id, __uuidof(IUnknown)) ||
				IsEqualIID(id, __uuidof(IDebugOutputCallbacks)))
			{
				*ppvObj = (IDebugOutputCallbacks *)this;
				AddRef();
				return S_OK;
			}
			else
			{
				return E_NOINTERFACE;
			}
		}

		virtual HRESULT _stdcall Output(IN ULONG Mask, IN PCSTR Text)
		{
			/*
			int len = strlen(Text);
			char *out = (char *)malloc(len + 128);
			char typ[128] = " ";

			if (Mask & DEBUG_OUTPUT_NORMAL)
				strcat(typ, "NORMAL ");
			else if (Mask & DEBUG_OUTPUT_ERROR)
				strcat(typ, "ERROR ");
			else if (Mask & DEBUG_OUTPUT_WARNING)
				strcat(typ, "WARNING ");
			else if (Mask & DEBUG_OUTPUT_VERBOSE)
				strcat(typ, "VERBOSE ");
			else if (Mask & DEBUG_OUTPUT_PROMPT)
				strcat(typ, "PROMPT ");
			else if (Mask & DEBUG_OUTPUT_PROMPT_REGISTERS)
				strcat(typ, "PROMPT_REGISTERS ");
			else if (Mask & DEBUG_OUTPUT_DEBUGGEE)
				strcat(typ, "DEBUGGEE ");
			else if (Mask & DEBUG_OUTPUT_SYMBOLS)
				strcat(typ, "SYMBOLS ");
			else if (Mask & DEBUG_OUTPUT_EXTENSION_WARNING)
				strcat(typ, "EXTENSION_WARNING ");
			else if (Mask & DEBUG_OUTPUT_DEBUGGEE_PROMPT)
				strcat(typ, "DEBUGGEE_PROMPT ");

			sprintf(out, "[%s] %s", typ, Text);
			puts(out);

			free(out);
			*/
			return 0;
		}
	};//Class Debugger::OutputCallbacks END
	class EventCallback : public IDebugEventCallbacks
	{
	public:
		// IUnknown.
		virtual ULONG _stdcall AddRef()
		{
			return 0;
		}
		virtual ULONG _stdcall Release()
		{
			return 1;
		}
		virtual HRESULT _stdcall QueryInterface(REFIID id, void **ppvObj)
		{
			*ppvObj = this;
			return NOERROR;
		}


		virtual HRESULT _stdcall GetInterestMask(PULONG Mask)
		{
			return S_OK;
		}
		virtual HRESULT _stdcall Breakpoint(IDebugBreakpoint *bp)
		{
			return DEBUG_STATUS_GO_HANDLED;
		}
		virtual HRESULT _stdcall ChangeDebuggeeState(ULONG Flags, ULONG64 Argument)
		{
			return DEBUG_STATUS_GO_HANDLED;
		}
		virtual HRESULT _stdcall ChangeEngineState(ULONG Flags, ULONG64 Argument)
		{
			return DEBUG_STATUS_GO_HANDLED;
		}
		virtual HRESULT _stdcall Exception(PEXCEPTION_RECORD64 Exception, ULONG FirstChance)
		{
			return DEBUG_STATUS_GO_HANDLED;
		}
		virtual HRESULT _stdcall UnloadModule(PCSTR ImageBaseName, ULONG64 BaseOffset)
		{
			return DEBUG_STATUS_GO_HANDLED;
		}
		virtual HRESULT _stdcall ExitProcess(ULONG ExitCode)
		{
			return DEBUG_STATUS_GO_HANDLED;
		}
		virtual HRESULT _stdcall SessionStatus(ULONG Status)
		{
			return DEBUG_STATUS_GO_HANDLED;
		}
		virtual HRESULT _stdcall ChangeSymbolState(ULONG Flags, ULONG64 Argument)
		{
			return DEBUG_STATUS_GO_HANDLED;
		}
		virtual HRESULT _stdcall SystemError(ULONG Error, ULONG Level)
		{
			return DEBUG_STATUS_GO_HANDLED;
		}
		virtual HRESULT _stdcall CreateThread(ULONG64 Handle, ULONG64 DateOffset, ULONG64 StartOffset)
		{
			printf("[Event:]Thread baseAddr:0x%x\n", StartOffset);
			return DEBUG_STATUS_GO_HANDLED;
		}
		virtual HRESULT _stdcall ExitThread(ULONG ExitCode)
		{
			return DEBUG_STATUS_GO_HANDLED;
		}
		virtual HRESULT _stdcall LoadModule(
			IN ULONG64  ImageFileHandle,
			IN ULONG64  BaseOffset,
			IN ULONG  ModuleSize,
			IN PCSTR  ModuleName,
			IN PCSTR  ImageName,
			IN ULONG  CheckSum,
			IN ULONG  TimeDateStamp)
		{
			printf("[Event:]Load module:%s\n", ModuleName);
			return DEBUG_STATUS_GO_HANDLED;
		}


		virtual HRESULT _stdcall CreateProcess(
			IN ULONG64  ImageFileHandle,
			IN ULONG64  Handle,
			IN ULONG64  BaseOffset,
			IN ULONG  ModuleSize,
			IN PCSTR  ModuleName,
			IN PCSTR  ImageName,
			IN ULONG  CheckSum,
			IN ULONG  TimeDateStamp,
			IN ULONG64  InitialThreadHandle,
			IN ULONG64  ThreadDataOffset,
			IN ULONG64  StartOffset
			)
		{
			return S_OK;
		}

	};


private:
	IDebugClient      *m_Client;
	IDebugControl     *m_Control;
	IDebugDataSpaces  *m_Data;
	IDebugRegisters   *m_Register;
	IDebugSymbols     *m_Symbol;

	PSTR m_cmd;
	ULONG m_flags;
	ULONG64 m_srv;

	void InitInterface();
};//Class Debugger END


#endif
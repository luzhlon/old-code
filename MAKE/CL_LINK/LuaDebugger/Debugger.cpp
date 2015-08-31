// Debugger.cpp : 定义应用程序的入口点。
//
#include "Debugger.h"

Debugger::Debugger(PSTR cmd, ULONG flags, ULONG64 srv)
	:m_cmd(cmd), m_flags(flags), m_srv(srv)
{
	InitInterface();
	if (cmd)
	{
		this->CreateProcess(cmd, flags, srv);
	}
}

Debugger::Debugger(ULONG id, ULONG flags, ULONG64 srv)
{
	InitInterface();
	if (id)
	{
		this->AttachProcess(id, flags, srv);
	}
}

void Debugger::InitInterface()
{
	//Create Debug Interface
	HRESULT hr;
	m_Client = NULL;
	m_Control = NULL;
	m_Data = NULL;
	m_Register = NULL;
	m_Symbol = NULL;
    hr = DebugCreate(__uuidof(IDebugClient), (void **)&m_Client);
	ASSERT(hr == S_OK, L"Create IDebugClient failue.\n");

	hr = m_Client->QueryInterface(__uuidof(IDebugControl), (void **)&m_Control);
	ASSERT(hr == S_OK, L"Create IDebugControl failue.\n");

	hr = m_Client->QueryInterface(__uuidof(IDebugDataSpaces), (void **)&m_Data);
	ASSERT(hr == S_OK, L"Create IDebugDataSpaces failue.\n");

	hr = m_Client->QueryInterface(__uuidof(IDebugRegisters), (void **)&m_Register);
	ASSERT(hr == S_OK, L"Create IDebugRegisters failue.\n");

	hr = m_Client->QueryInterface(__uuidof(IDebugSymbols), (void **)&m_Symbol);
	ASSERT(hr == S_OK, L"Create IDebugSymbols failue.\n");
	//END Interface Create
}

int Debugger::CreateProcess(PSTR cmd, ULONG flags, ULONG64 srv)
{
	HRESULT hr;
	hr = m_Client->CreateProcessW(srv, cmd, flags);
	return hr == S_OK;
}

int Debugger::AttachProcess(ULONG id, ULONG flags, ULONG64 srv)
{
	HRESULT hr;
	hr = m_Client->AttachProcess(srv, id, flags);
	return hr == S_OK;
}

Debugger::~Debugger()
{
	if (!m_Client)return;
	m_Client->Release();
}
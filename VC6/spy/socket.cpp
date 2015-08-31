#include "stdafx.h"
#include "socket.h"

MSocket::MSocket(short nPort, char *szIP)
{
	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);			//≥ı ºªØWSAø‚

	Port = nPort;
	IP = szIP;

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sOld = s;
}

inline short MSocket::getPort()
{
	return Port;
}

inline char* MSocket::getIP()
{
	return IP;
}

void MSocket::setIP(char* szIP)
{
	IP = szIP;
}

void MSocket::setPort(short nPort)
{
	Port = nPort;
}

inline SOCKET MSocket::getSocket()
{
	return s;
}

int MSocket::bind()
{
	sockaddr_in localAddr;
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(Port);
	localAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	
	return ::bind(s, (LPSOCKADDR)&localAddr, sizeof(localAddr));
}

int MSocket::listen()
{
	return ::listen(s, 5);
}

int MSocket::connect()
{
	sockaddr_in remoteAddr;
	remoteAddr.sin_family = AF_INET;
	remoteAddr.sin_port = htons(Port);
	remoteAddr.sin_addr.S_un.S_addr = inet_addr(IP);

	return ::connect(s, (LPSOCKADDR)&remoteAddr, sizeof(remoteAddr));
}

void MSocket::accept()
{
	sockaddr_in remoteAddr;
	int nAddrLen = sizeof(remoteAddr);
	s = ::accept(sOld, (LPSOCKADDR)&remoteAddr, &nAddrLen);

	Port = ntohs(remoteAddr.sin_port);
	IP = inet_ntoa(remoteAddr.sin_addr);
}

int MSocket::send(char *buf, int len)
{
	return ::send(s, buf, len, 0);
}

int MSocket::recv(char *buf, int len)
{
	return ::recv(s, buf, len, 0);
}








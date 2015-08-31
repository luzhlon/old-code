#include <WinSock2.h>

#ifndef __SOCKET_H__
#define __SOCKET_H__

#pragma comment(lib, "Ws2_32.lib")

class MSocket
{
public:
	MSocket(short nPort=0, char *szIP=0);		//¹¹Ôìº¯Êý

	int bind();
	int listen();
	int connect();
	void accept();
	int send(char *, int);
	int recv(char *, int);

	inline short getPort();
	void setPort(short nPort=0);
	inline char* getIP();
	void setIP(char *szIP=0);
	inline SOCKET getSocket();
private:
	short Port;
	char  *IP;
	SOCKET s;
	SOCKET sOld;

};

#endif
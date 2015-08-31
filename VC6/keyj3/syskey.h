///////////////////////////////////////////////////////
//Çý¶¯¼üÅÌ¶Ë¿Ú
//////////////////////////////////////////////////////

#ifndef __WINDOWS_H__
#include "windows.h"
#endif

class MKeyBoard
{
public:
	MKeyBoard();
	void up(char);
	void down(char);
	void click(char);
private:
	HANDLE hDevice;
};
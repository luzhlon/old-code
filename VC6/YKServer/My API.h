//创建打开子键
DWORD	AppCreateKey(HKEY ParentKey,char *szSubKey,HKEY *phSubKey)
{
	return RegCreateKeyEx(ParentKey,szSubKey,0,NULL,REG_OPTION_VOLATILE,KEY_ALL_ACCESS,NULL,phSubKey,NULL);
}


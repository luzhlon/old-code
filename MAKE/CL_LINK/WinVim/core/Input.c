
struct luaL_reg winvim_core[] = 
{
    //Keyboard-about functions
	{"DownKey", LuaDownKey},
	{"UpKey", LuaUpKey},
	{"IsKeyDown", LuaIsKeyDown},
	{"IsKeyToggled", LuaIsKeyToggled},
	{"KeyToChar", LuaKeyToChar},
    //Mouse-about functions
	{"GetCursorPos", LuaGetCursorPos},
	{"MouseMove", LuaMouseMove},
	{"MouseDown", LuaMouseDown},
	{"MouseUp", LuaMouseUp},
	{"MouseWheel", LuaMouseWheel},
    //Hook-Control functions
	{"TurnOn", LuaTurnOn},
	{"TurnOff", LuaTurnOff},
	{"SetActiveKeys", LuaSetActiveKeys},
	{"InstallHook", LuaInstallHook},
	{"UninstallHook", LuaUninstallHook},
    //Window-about functions
	{"GetTopWindows", LuaGetTopWindows},
	{"GetChildWindows", LuaGetChildWindows},
	{"GetWindowName", LuaGetWindowName},
    //Extension functions
	{"NewThread", LuaNewThread},
	{"Sleep", LuaSleep},
	{"TextOut", LuaTextOut},
    {0, 0}
};
int __declspec(dllexport) luaopen_Input(lua_State *L)
{
    g_LuaState = L;
    g_hookState = STATE_OFF;
	luaL_openlib(L, szLibName, winvim_core, 0);
	return 1;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            g_hInstance = hinstDLL;
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
        case DLL_THREAD_DETACH:
            break;
    }
    return 1;
}

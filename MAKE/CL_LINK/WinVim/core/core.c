#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <stdio.h>
#include <windows.h>

HINSTANCE     g_hInstance = 0;
HHOOK         g_hHook = 0;
HWND          g_hWnd;
BYTE          g_kbState[256] = {0};
lua_State     *g_LuaState;
char szLibName[] = "winvim_core";
//函数声明
void DrawTextOnScreen(char *);
int  IsKeyDown(BYTE );
//
//--------------一些功能性的函数------------------
//
//按下一个键
void DownKey(BYTE vk)
{
    keybd_event(vk, MapVirtualKey(vk, 0), 0, 0);
}
//松开一个键
void UpKey(BYTE vk)
{
    keybd_event(vk, MapVirtualKey(vk, 0), KEYEVENTF_KEYUP, 0);
}
//移动鼠标
void MouseMove(int x, int y)
{
    mouse_event(MOUSEEVENTF_MOVE, x, y, 0, 0);
}
//按下鼠标
void MouseDown(char key)
{
    int flags = 0;
    switch(key)
    {
        case 'r':
            flags |= MOUSEEVENTF_RIGHTDOWN;
            break;
        case 'l':
            flags |= MOUSEEVENTF_LEFTDOWN;
            break;
        case 'm':
            flags |= MOUSEEVENTF_MIDDLEDOWN;
            break;
        default:
            DrawTextOnScreen("Unkown mouse key.\n");
            return;
    }
    mouse_event(flags, 0, 0, 0, 0);
}
//松开鼠标
void MouseUp(char key)
{
    int flags = 0;
    switch(key)
    {
        case 'r':
            flags |= MOUSEEVENTF_RIGHTUP;
            break;
        case 'l':
            flags |= MOUSEEVENTF_LEFTUP;
            break;
        case 'm':
            flags |= MOUSEEVENTF_MIDDLEUP;
            break;
        default:
            DrawTextOnScreen("Unkown mouse key.\n");
            return;
    }
    mouse_event(flags, 0, 0, 0, 0);
}
//鼠标滚轮滚动
void MouseWheel(int range)
{
    int flags = MOUSEEVENTF_WHEEL;
    mouse_event(flags, 0, 0, range, 0);
}
//判断一个键是否按下
int IsKeyDown(BYTE vk)
{
    int State = GetKeyState(vk);
    return ((State)>>(sizeof(int)*8-1));
    //return g_kbState[vk];
}
//判断状态键(CapsLock, NumLock,..)
int IsKeyToggled(BYTE vk)
{
    int State = GetKeyState(vk);
    return State & 1;
}

void DrawTextOnScreen(char *text)
{
    HWND hdesk = GetDesktopWindow();
    RECT rc;

    HDC deskHdc = GetDC(hdesk);
    GetWindowRect(hdesk, &rc);
    DrawText(deskHdc,
    "                                                                                 ",
        -1, &rc, 0); 
    DrawText(deskHdc, text, -1, &rc, 0); 
    ReleaseDC(hdesk,deskHdc);
}

int CallLuaScript(BYTE vkCode, int bDown)
{
    lua_getglobal(g_LuaState, "_CALLBACK");
    lua_pushinteger(g_LuaState, vkCode);
    lua_pushboolean(g_LuaState, bDown);
    lua_pcall(g_LuaState, 2, 1, 0);
    return lua_toboolean(g_LuaState, -1);
}

//LowLevel keyboard Hook callback function
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM kbMsg, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT pKbDll = (PKBDLLHOOKSTRUCT)lParam;
    int bDown = 0;
    static int bInLua = 0;

    switch(kbMsg)
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        g_kbState[pKbDll->vkCode] = 1;
        bDown = 1;
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        g_kbState[pKbDll->vkCode] = 0;
        bDown = 0;
        break;
    default:
        bDown = -1;
        DrawTextOnScreen("Unknown WM_KEY message.");
        return 1;
    }

    if(nCode == HC_ACTION)
    {
        if(bInLua)return CallNextHookEx(g_hHook, nCode, kbMsg, lParam);

        bInLua = 1;
        int bDealed = CallLuaScript((BYTE)pKbDll->vkCode, bDown>0);
        bInLua = 0;
        if(bDealed)return 1;
    }

    return CallNextHookEx(g_hHook, nCode, kbMsg, lParam);
}

void InstallHook()
{
    g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, g_hInstance, 0);
    if(!g_hHook)
    {
        DrawTextOnScreen("Install Hook failue.\n");
    }
    else
    {
        DrawTextOnScreen("Install Success.\n");
    }
}

void UninstallHook()
{
    if(g_hHook)UnhookWindowsHookEx(g_hHook);
}

//为Lua层提供的函数
//--LuaLayout
int LuaInstallHook(lua_State *L)
{
    InstallHook();
    return 0;
}
int LuaUninstallHook(lua_State *L)
{
    UninstallHook();
    return 0;
}
int LuaDownKey(lua_State *L)
{
    DownKey(lua_tointeger(L, 1));
    return 0;
}
int LuaUpKey(lua_State *L)
{
    UpKey(lua_tointeger(L, 1));
    return 0;
}
int LuaGetCursorPos(lua_State *L)
{
    POINT csp;
    GetCursorPos(&csp);
    lua_pushinteger(L, csp.x);
    lua_pushinteger(L, csp.y);
    return 2;
}
int LuaMouseMove(lua_State *L)
{
    int x = lua_tointeger(L, 1);
    int y = lua_tointeger(L, 2);
    MouseMove(x, y);
    return 0;
}
int LuaMouseDown(lua_State *L)
{
    char *key = lua_tostring(L, 1);
    MouseDown(*key);
    return 0;
}
int LuaMouseUp(lua_State *L)
{
    char *key = lua_tostring(L, 1);
    MouseUp(*key);
    return 0;
}
int LuaMouseWheel(lua_State *L)
{
    MouseWheel(lua_tointeger(L, 1));
    return 0;
}
int LuaIsKeyDown(lua_State *L)
{
    lua_pushboolean(L, IsKeyDown(lua_tointeger(L, 1)));
    return 1;
}
int LuaIsKeyToggled(lua_State *L)
{
    lua_pushboolean(L, IsKeyToggled(lua_tointeger(L, 1)));
    return 1;
}
int LuaKeyToChar(lua_State *L)
{
    BYTE kbstate[256];
    char str[32];
    BYTE vkCode = lua_tointeger(L, 1);
    GetKeyboardState(kbstate);
    kbstate[VK_SHIFT] = GetKeyState(VK_SHIFT);
    int r = ToAscii(vkCode, MapVirtualKey(vkCode, 0), kbstate, str, 1);
    str[r] = 0;
    lua_pushstring(L, str);
    return 1;
}

int LuaSleep(lua_State *L)
{
    Sleep(lua_tonumber(L, 1)*1000);
    return 0;
}
int LuaTextOut(lua_State *L)
{
    DrawTextOnScreen(lua_tostring(L, 1));
    return 0;
}
DWORD WINAPI LuaThreadProc(LPVOID lpParameter)
{
    lua_State *L = (lua_State *)lpParameter;
    lua_State *newL = lua_newstate(L, 0);
    int nTop = lua_gettop(L);
    lua_xmove(L, newL, nTop);
    if(lua_isfunction(newL, 1))
    {
        lua_pcall(newL, nTop -1 , 0, 0);
    }
    else
    {
        lua_error(newL);
    }
    return 0;
}
int LuaNewThread(lua_State *L)
{
    DWORD  id;
    HANDLE h = CreateThread(0, 0, LuaThreadProc, L, 0, &id);
    if(h)
    {
        lua_pushboolean(L, 1);
        lua_pushinteger(L, id);
        return 2;
    }
    else
    {
        lua_pushboolean(L, 0);
        return 1;
    }
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    static int index = 1;
    lua_State *L = (lua_State *)lParam;

    lua_pushinteger(L, index);         //key     -2
    lua_pushinteger(L, (int)hwnd);     //value   -1
    lua_settable(L, -3);

    index ++;
    return 1;
}

int LuaGetTopWindows(lua_State *L)
{
    lua_newtable(L);
    EnumWindows(EnumWindowsProc, (LPARAM)L);
    return 1;
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
    static int index = 1;
    lua_State *L = (lua_State *)lParam;

    lua_pushinteger(L, index);         //key      -2
    lua_pushinteger(L, (int)hwnd);     //value    -1
    lua_settable(L, -3);

    index ++;
    return 1;
}

int LuaGetChildWindows(lua_State *L)
{
    lua_newtable(L);
    HWND hParent = (HWND)lua_tointeger(L, 1);
    EnumChildWindows(hParent, EnumChildProc, (LPARAM)L);
    return 1;
}

int LuaGetWindowName(lua_State *L)
{
    char wndName[1024];
    HWND hWnd = (HWND)lua_tointeger(L, 1);
    GetWindowText(hWnd, wndName, 1024);
    lua_pushstring(L, wndName);
    return 1;
}

int LuaRegisterHotKey(lua_State *L)
{
    UINT ExKey = 0;
    BYTE vk = lua_tointeger(L, 1);
    char *ek = lua_tostring(L, 2);
    if(ek)
    {
        while(*ek)
            switch(*ek++)
            {
            case 'c':
            case 'C':
                ExKey |= MOD_CONTROL;
                break;
            case 'a':
            case 'A':
                ExKey |= MOD_ALT;
                break;
            case 's':
            case 'S':
                ExKey |= MOD_SHIFT;
                break;
            case 'w':
            case 'W':
                ExKey |= MOD_WIN;
                break;
            }
    }
    lua_pushboolean(L, RegisterHotKey(g_hWnd, 0, ExKey, vk));
    return 1;
}

int LuaMsgLoop(lua_State *L)
{
    MSG msg;
    while (GetMessage (&msg, NULL, 0, 0))
    {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
}

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
	{"InstallHook", LuaInstallHook},
	{"UninstallHook", LuaUninstallHook},
    //Window-about functions
	{"GetTopWindows", LuaGetTopWindows},
	{"GetChildWindows", LuaGetChildWindows},
	{"GetWindowName", LuaGetWindowName},
    //Core functions
	{"RegisterHotKey", LuaRegisterHotKey},
	{"MsgLoop", LuaMsgLoop},
    //Extension functions
	{"NewThread", LuaNewThread},
	{"Sleep", LuaSleep},
	{"TextOut", LuaTextOut},
    {0, 0}
};
int __declspec(dllexport) luaopen_winvim_core(lua_State *L)
{
    int WvCreateWindow();
    g_LuaState = L;
	luaL_openlib(L, szLibName, winvim_core, 0);
    WvCreateWindow();//创建窗口，用于消息循环以及注册热键
	return 1;
}

int WvCreateWindow()
{
    char *szWinName = "Vim to Windows";
    char *szClassName = "WinVim";
    LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
    HWND         hwnd;
    MSG          msg;
    WNDCLASS     wndclass;

    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = g_hInstance;
    wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
    wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
    wndclass.lpszMenuName  = NULL;
    wndclass.lpszClassName = szClassName; 

    if (!RegisterClass (&wndclass))
    {
        MessageBox (NULL, TEXT ("This program requires Windows NT!"), 
                  szWinName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow (szClassName,                  // window class name
                      szWinName,
                      WS_OVERLAPPEDWINDOW | WS_EX_TRANSPARENT,        // window style
                      CW_USEDEFAULT,              // initial x position
                      CW_USEDEFAULT,              // initial y position
                      CW_USEDEFAULT,              // initial x size
                      CW_USEDEFAULT,              // initial y size
                      NULL,                       // parent window handle
                      NULL,                       // window menu handle
                      g_hInstance,                  // program instance handle
                      NULL);                     // creation parameters

    g_hWnd = hwnd;

    //ShowWindow (hwnd, 1);
    UpdateWindow (hwnd);

    return 0;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
            return 0;

        case WM_PAINT:
            return 0;

        case WM_HOTKEY:
            DrawTextOnScreen("Hotkey occured");
            InstallHook();
            return 0;

        case WM_DESTROY:
            PostQuitMessage (0);
            return 0;
    }
    return DefWindowProc (hwnd, message, wParam, lParam);
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

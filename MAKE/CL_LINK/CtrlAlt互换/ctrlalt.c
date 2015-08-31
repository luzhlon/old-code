#include <windows.h>

//HANDLE g_EventHandle;
HHOOK  g_hHook = NULL;
HWND   g_hWnd;
HINSTANCE g_hInstance;
const char *cap = "Ctrl-Alt »¥»» V1.0";

int DealCtrlAlt(int bCtrl, int bDown)
{
    if(bCtrl)
    {
        BYTE vk = VK_LMENU;
        if(bDown)
            keybd_event(vk, MapVirtualKey(vk, 0), NULL, 0);
        else
            keybd_event(vk, MapVirtualKey(vk, 0), KEYEVENTF_KEYUP, 0);
    }
    else
    {
        BYTE vk = VK_LCONTROL;
        if(bDown)
            keybd_event(vk, MapVirtualKey(vk, 0), NULL, 0);
        else
            keybd_event(vk, MapVirtualKey(vk, 0), KEYEVENTF_KEYUP, 0);
    }
    return 0;
}


LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM kbMsg, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT pKbDll = (PKBDLLHOOKSTRUCT)lParam;
    static int bInVitual = 0;

#define ENTER bInVitual = 1
#define LEAVE bInVitual = 0;return 1

    if(bInVitual)goto RET;

    switch(kbMsg)
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        switch(pKbDll->vkCode)
        {
        case VK_LMENU:
            ENTER;
            DealCtrlAlt(0, 1);
            LEAVE; 
        case VK_LCONTROL:
            ENTER;
            DealCtrlAlt(1, 1);
            LEAVE;
        default:
            goto RET;
        }
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        switch(pKbDll->vkCode)
        {
        case VK_LMENU:
            ENTER;
            DealCtrlAlt(0, 0);
            LEAVE;
        case VK_LCONTROL:
            ENTER;
            DealCtrlAlt(1, 0);
            LEAVE;
        default:
            goto RET;
        }
        break;
    }

RET:
    return CallNextHookEx(g_hHook, nCode, kbMsg, lParam);
}

void InstallHook()
{
    g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, g_hInstance, 0);
    if(!g_hHook)
    {
        MessageBox(0, "Hook FAILUE!", cap, 0);
    }
}

void UninstallHook()
{
    if(g_hHook)UnhookWindowsHookEx(g_hHook);
    g_hHook = NULL;
}

void MsgLoop()
{
    MSG msg;
    while (GetMessage (&msg, NULL, 0, 0))
    {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
            if(!RegisterHotKey(hwnd, 0, MOD_WIN, 'A'))
                MessageBox(hwnd, "RegisterHotKey FAILUE!", cap, 0);
            MessageBox(hwnd, "RegisterHotKey Success!", cap, 0);
            return 0;

        case WM_PAINT:
            return 0;

        case WM_HOTKEY:
            if(g_hHook)
            {
                UninstallHook();
                MessageBox(hwnd, "UninstallHooked", cap, 0);
            }
            else
            {
                InstallHook();
                MessageBox(hwnd, "InstallHooked", cap, 0);
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage (0);
            return 0;
    }
    return DefWindowProc (hwnd, message, wParam, lParam);
}

void CreateWindow_()
{
    char *szWinName = cap;
    char *szClassName = "CtrlAlt Exchange winClass";
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

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    g_hInstance = hInstance;
    CreateWindow_();
    InstallHook();
    MsgLoop();
    return 0;
}


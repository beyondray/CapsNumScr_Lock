// CapsNumScr_Lock.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "CapsNumScr_Lock.h"
#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE g_hInst;								// 当前实例
HWND g_hWnd;
NotifyIcon g_CapsIcon;
NotifyIcon g_NumIcon;
NotifyIcon g_ScrIcon;
typedef const wchar_t * STRSAFE_LPCSTR;
STRSAFE_LPCSTR g_lpCapsLockTips		=	TEXT("大写锁定");
STRSAFE_LPCSTR g_lpCapsUnLockTips	=	TEXT("小写锁定");
STRSAFE_LPCSTR g_lpNumLockTips		=	TEXT("锁定数字键");
STRSAFE_LPCSTR g_lpNumUnLockTips	=	TEXT("锁定方向键");
STRSAFE_LPCSTR g_lpScrLockTips		=	TEXT("滚动锁屏");
STRSAFE_LPCSTR g_lpScrUnLockTips	=	TEXT("滚动解屏");

TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
VOID				ChangeAllIcon();
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	NotifyIconProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void				ShowContextMenu(HWND hwnd, POINT pt);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//禁用高DPI显示缩放
	HINSTANCE hUser32 = LoadLibrary("user32.dll");
	if (hUser32)
	{
		typedef BOOL(WINAPI* LPSetProcessDPIAware)(void);
		LPSetProcessDPIAware pSetProcessDPIAware = (LPSetProcessDPIAware)GetProcAddress(hUser32,
			"SetProcessDPIAware");
		if (pSetProcessDPIAware)
		{
			pSetProcessDPIAware();
		}
		FreeLibrary(hUser32);
	}

 	// TODO: 在此放置代码。
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CAPSNUMSCR_LOCK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CAPSNUMSCR_LOCK));

	// 主消息循环:
	MSG msg = {0};
	while(msg.message!= WM_QUIT)
	{
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else{
			Sleep(100);
			ChangeAllIcon();
		}

	}
	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW|CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CAPSNUMSCR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CAPSNUMSCR_LOCK);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_CAPSNUMSCR));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{


   g_hInst = hInstance; // 将实例句柄存储在全局变量中

   g_hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!g_hWnd)
   {
      return FALSE;
   }

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
VOID ChangeIconWithKeyState(int nVirKey, NotifyIcon& icon, LPCTSTR lpLockIconName, STRSAFE_LPCSTR lpLockTips,
							LPCTSTR lpUnLockIconName, STRSAFE_LPCSTR lpUnLockTips)
{
	SHORT s = GetKeyState(nVirKey);
	BYTE lowBit = s & 0x01;
	bool bLock = (lowBit == 1);
	if(bLock)
	{
		icon.modify(g_hInst, lpLockIconName, lpLockTips);
	}else
	{
		icon.modify(g_hInst, lpUnLockIconName, lpUnLockTips);
	}
}

VOID ChangeAllIcon()
{
	//Scr_Lock
	ChangeIconWithKeyState(VK_SCROLL, g_ScrIcon, MAKEINTRESOURCE(IDI_SCR_LOCK), 
		g_lpScrLockTips, MAKEINTRESOURCE(IDI_SCR_UNLOCK), g_lpScrUnLockTips);

	//Num_Lock
	ChangeIconWithKeyState(VK_NUMLOCK, g_NumIcon, MAKEINTRESOURCE(IDI_NUM_LOCK), 
		g_lpNumLockTips, MAKEINTRESOURCE(IDI_NUM_UNLOCK), g_lpNumUnLockTips);

	//Caps_Lock
	ChangeIconWithKeyState(VK_CAPITAL, g_CapsIcon, MAKEINTRESOURCE(IDI_CAPS_LOCK), 
		g_lpCapsLockTips, MAKEINTRESOURCE(IDI_CAPS_UNLOCK),g_lpCapsUnLockTips);
}

void CreateAllIcon(HWND hwnd)
{
	//========================【最后创建的显示在最前面】============================
	//Scr_Lock
	g_ScrIcon.create(g_hInst, hwnd, MAKEINTRESOURCE(IDI_SCR_LOCK), g_lpScrLockTips);

	//Num_Lock
	g_NumIcon.create(g_hInst, hwnd, MAKEINTRESOURCE(IDI_NUM_LOCK), g_lpNumLockTips);

	//Caps_Lock
	g_CapsIcon.create(g_hInst, hwnd, MAKEINTRESOURCE(IDI_CAPS_LOCK), g_lpCapsLockTips);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		ShowWindow(hWnd, SW_HIDE);
		CreateAllIcon(hWnd);
		ChangeAllIcon();
		break;

	case WM_KEYDOWN:
		ChangeAllIcon();
		break;
		
	case WMAPP_NOTIFYCALLBACK:
		NotifyIconProc(hWnd, message, wParam, lParam);
		break;
	
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//任务栏图标事件调用
LRESULT CALLBACK NotifyIconProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	switch (LOWORD(lParam))
	{
	case NIN_SELECT:
		break;

	case WM_CONTEXTMENU:
		//MessageBox(hWnd, TEXT("点击"), NULL, MB_OK);
		pt.x = LOWORD(wParam);
		pt.y = HIWORD(wParam);
		ShowContextMenu(hWnd, pt);
		break;

	default:
		break;
	}
	return 0;
}

void ShowContextMenu(HWND hwnd, POINT pt)
{
	HMENU hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDC_CONTENTMENU));
	if (hMenu)
	{
		HMENU hSubMenu = GetSubMenu(hMenu, 0);
		if (hSubMenu)
		{
			// our window must be foreground before calling TrackPopupMenu or the menu will not disappear when the user clicks away
			SetForegroundWindow(hwnd);

			// respect menu drop alignment
			UINT uFlags = TPM_RIGHTBUTTON;

			if (GetSystemMetrics(SM_MENUDROPALIGNMENT) != 0)
			{
				uFlags |= TPM_RIGHTALIGN;
			}
			else
			{
				uFlags |= TPM_LEFTALIGN;
			}

			TrackPopupMenuEx(hSubMenu, NULL, pt.x, pt.y, hwnd, NULL);
		}
		DestroyMenu(hMenu);
	}
}

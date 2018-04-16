#include "ShellNotify.h"


extern NOTIFYICONDATA nid;
extern HINSTANCE g_hInst;

NotifyIcon::NotifyIcon()
{

}

NotifyIcon::~NotifyIcon()
{
	clear();
}

BOOL NotifyIcon::create(HINSTANCE hInst, HWND hwnd, LPCTSTR lpIconName, STRSAFE_LPCSTR lpTips)
{
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	// add the icon, setting the icon, tooltip, and callback message.
	// the icon will be identified with the GUID
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_SHOWTIP | NIF_GUID;	
	
	
	static GUID myGUID = {0x23977b55, 0x10e0, 0x4041, {0xb8, 0x62, 0xb1, 0x95, 0x41, 0x96, 0x36, 0x69}};
	//static BOOL bSuc = createGUIDFromFile(myGUID);
	//if(!bSuc)
	//{
	//	coCreateGUIDAndWriteFile(myGUID);
	//}
	myGUID.Data1+=1;
	nid.guidItem = myGUID;

	// This text will be shown as the icon's tooltip.
	StringCchCopy(nid.szTip, ARRAYSIZE(nid.szTip), lpTips);

	// Load the icon for high DPI.
	nid.hIcon = LoadIcon(hInst, lpIconName);

	nid.uCallbackMessage = WMAPP_NOTIFYCALLBACK;
	
	//LoadString(g_hInst, TEXT("Test application"), nid.szTip, ARRAYSIZE(nid.szTip));
	Shell_NotifyIcon(NIM_ADD, &nid);

	// NOTIFYICON_VERSION_4 is prefered
	nid.uVersion = NOTIFYICON_VERSION_4;

	return Shell_NotifyIcon(NIM_SETVERSION, &nid);
}

BOOL NotifyIcon::modify(HINSTANCE hInst, LPCTSTR lpIconName, STRSAFE_LPCSTR lpTips)
{
	nid.hIcon =  LoadIcon(hInst, lpIconName);
	StringCchCopy(nid.szTip, ARRAYSIZE(nid.szTip), lpTips);
	return Shell_NotifyIcon(NIM_MODIFY, &nid);
}

BOOL NotifyIcon::clear()
{
	return Shell_NotifyIcon(NIM_DELETE, &nid);
}

BOOL NotifyIcon::createGUIDFromFile( GUID& guid )
{
	//将GUID记录文件打开
	if(!fstream.is_open())
	{
		fstream.open("./guid.dat", ios::in|ios::binary);
	}

	//读取GUID数据
	if(fstream.is_open())
	{
		if(!fstream.eof())
		{
			fstream>>guid.Data1;
			fstream>>guid.Data2;
			fstream>>guid.Data3;
			fstream>>guid.Data4;
			fstream.close();
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL NotifyIcon::coCreateGUIDAndWriteFile(GUID& guid)
{
	//if(fstream.is_open())
	//	fstream.close();

	fstream.open("./guid.dat", ios::out|ios::binary|ios::app);
	if(fstream.is_open())
	{
		CoCreateGuid(&guid);
		fstream << guid.Data1;
		fstream << guid.Data2;
		fstream << guid.Data3;
		fstream << guid.Data4;
		fstream.close();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}




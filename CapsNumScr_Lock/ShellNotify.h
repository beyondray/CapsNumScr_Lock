#pragma once

#include <windows.h>
#include "resource.h"

#include <shellapi.h>
#pragma comment(lib, "Shell32.lib")

#include <Strsafe.h>

#include <fstream>
using namespace std;

class NotifyIcon
{
public:
	NotifyIcon();
	~NotifyIcon();

	BOOL create(HINSTANCE hInst, HWND hwnd, LPCTSTR lpIconName, STRSAFE_LPCSTR lpTips);
	BOOL modify(HINSTANCE hInst, LPCTSTR lpIconName, STRSAFE_LPCSTR lpTips);
	BOOL clear();

	NOTIFYICONDATA& getNID(){return nid;}

private:
	BOOL createGUIDFromFile(GUID& guid);
	BOOL coCreateGUIDAndWriteFile(GUID& guid);

private:
	NOTIFYICONDATA nid;
	std::fstream fstream;
};




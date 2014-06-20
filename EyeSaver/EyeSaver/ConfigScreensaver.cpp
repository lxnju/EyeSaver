#include "stdafx.h"
#include "ConfigScreensaver.h"

#define ScreenSaverKey L"Control Panel\\Desktop"
#define BubbleSS	L"C:\\Windows\\system32\\Bubbles.scr"


CConfigScreensaver::CConfigScreensaver(void)
{
}


CConfigScreensaver::~CConfigScreensaver(void)
{
}

bool CConfigScreensaver::ConfigBubble()
{
	HKEY hkey;
	DWORD dwres = RegOpenKeyEx(HKEY_CURRENT_USER, ScreenSaverKey, 0, KEY_SET_VALUE, &hkey);
	if (dwres != ERROR_SUCCESS)
	{
		return false;
	}

	dwres = RegSetValueEx(hkey, L"ScreenSaveActive", 0, REG_SZ, (LPBYTE)L"1", (_tcslen(L"1") + 1)*sizeof(TCHAR));  
	if (dwres != ERROR_SUCCESS)
	{
		return false;
	}

	dwres = RegSetValueEx(hkey, L"SCRNSAVE.EXE", 0, REG_SZ, (LPBYTE)BubbleSS, (_tcslen(BubbleSS) + 1)*sizeof(TCHAR));  
	if (dwres != ERROR_SUCCESS)
	{
		return false;
	}

	RegCloseKey(hkey);

	return true;
}
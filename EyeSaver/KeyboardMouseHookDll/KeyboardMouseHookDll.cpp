// MouseHookDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

HHOOK g_LowLevelMouseHook = NULL;
HHOOK g_keyboardHook = NULL;

LRESULT CALLBACK LowLevelMouseProc(
  _In_  int code,
  _In_  WPARAM wParam,
  _In_  LPARAM lParam
);

LRESULT CALLBACK KeyboardProc(
  _In_  int code,
  _In_  WPARAM wParam,
  _In_  LPARAM lParam
);


extern HMODULE g_mod;

__declspec(dllexport) void SetKeyboardMouseHook()
{
	g_keyboardHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_mod, 0);
	if (!g_keyboardHook)
	{
		OutputDebugString(L"failed to set keyboard hook");
		return;
	}
	g_LowLevelMouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, g_mod, 0);
	if (!g_LowLevelMouseHook)
	{
		OutputDebugString(L"failed to set mouse hook");
	}
}

__declspec(dllexport) void RemoveKeyboardMouseHook()
{
	UnhookWindowsHookEx(g_keyboardHook); 
	UnhookWindowsHookEx(g_LowLevelMouseHook); 
}

LRESULT CALLBACK LowLevelMouseProc(
  _In_  int code,
  _In_  WPARAM wParam,
  _In_  LPARAM lParam
)
{ 
	if (code<0 )
		CallNextHookEx(g_LowLevelMouseHook, code, wParam, lParam);
	
	// filter all mouse message
	return 1; 	
} 

LRESULT CALLBACK KeyboardProc(
  _In_  int code,
  _In_  WPARAM wParam,
  _In_  LPARAM lParam
)
{ 
	if (code<0 )
		CallNextHookEx(g_keyboardHook, code, wParam, lParam);
	
	// filter all key message except esc
	if (wParam == VK_ESCAPE)
	{
		RemoveKeyboardMouseHook();
		return CallNextHookEx(g_LowLevelMouseHook, code, wParam, lParam);
	}	
	return 1; 	
} 
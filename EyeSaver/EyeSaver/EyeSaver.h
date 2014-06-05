
// EyeSaver.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CEyeSaverApp:
// See EyeSaver.cpp for the implementation of this class
//

class CEyeSaverApp : public CWinApp
{
public:
	CEyeSaverApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CEyeSaverApp theApp;
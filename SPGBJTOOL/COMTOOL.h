
// COMTOOL.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCOMTOOLApp:
// �йش����ʵ�֣������ COMTOOL.cpp
//

#define WM_PROTOCOLFINISH (WM_USER + 11)
class CCOMTOOLApp : public CWinApp
{
public:
	CCOMTOOLApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCOMTOOLApp theApp;
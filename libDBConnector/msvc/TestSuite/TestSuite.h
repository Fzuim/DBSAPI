
// TestSuite.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestSuiteApp:
// �йش����ʵ�֣������ TestSuite.cpp
//

class CTestSuiteApp : public CWinAppEx
{
public:
	CTestSuiteApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestSuiteApp theApp;
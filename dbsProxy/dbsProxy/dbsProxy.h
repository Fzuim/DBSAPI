// dbsProxy.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "ArLog.h"

// CdbsProxyApp:
// �йش����ʵ�֣������ dbsProxy.cpp
//

class CdbsProxyApp : public CWinApp
{
public:
	CdbsProxyApp();
	CLog m_Log;
// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};
// dbsInterface.h : dbsInterface DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CdbsInterfaceApp
// �йش���ʵ�ֵ���Ϣ������� dbsInterface.cpp
//

class CdbsInterfaceApp : public CWinApp
{
public:
	CdbsInterfaceApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

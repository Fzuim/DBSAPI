// dbsProxyDlg.h : ͷ�ļ�
//

#pragma once
#include "EnvMonEvent.h"

// CdbsProxyDlg �Ի���
class CdbsProxyDlg : public CDialog
{
// ����
public:
	CdbsProxyDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DBSPROXY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	LRESULT DealRefreshMsg(WPARAM v_wParam, LPARAM v_lParam);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnTestoracle();
	afx_msg void OnBnClickedBtnTestmysql();
	afx_msg void OnBnClickedBtnEnvmon();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

private:
	CEnvMonEvent* m_pEnvMonEvent;

	void InitUI();
};

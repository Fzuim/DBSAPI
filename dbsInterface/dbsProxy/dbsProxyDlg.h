
// dbsProxyDlg.h : ͷ�ļ�
//

#pragma once

typedef void (WINAPI* PDLLTEST)(BYTE);

// CdbsProxyDlg �Ի���
class CdbsProxyDlg : public CDialogEx
{
// ����
public:
	CdbsProxyDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DBSPROXY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnTest();
	BOOL InitDBInterface();

private:
	BOOL m_bDBInterfaceInited;
	HMODULE m_hDBInterface;

	PDLLTEST m_pDLLTest;
};

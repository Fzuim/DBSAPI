
// dbsProxyDlg.h : 头文件
//

#pragma once

typedef void (WINAPI* PDLLTEST)(BYTE);

// CdbsProxyDlg 对话框
class CdbsProxyDlg : public CDialogEx
{
// 构造
public:
	CdbsProxyDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DBSPROXY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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

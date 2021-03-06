
// PcShutDownByTimeDlg.h: 头文件
//

#pragma once


// CPcShutDownByTimeDlg 对话框
class CPcShutDownByTimeDlg : public CDialogEx
{
// 构造
public:
	CPcShutDownByTimeDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PCSHUTDOWNBYTIME_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnDisplayTime(WPARAM wParam, LPARAM lParam);
	CEdit m_currenttime;
	CComboBox m_shutdownhour;
	CComboBox m_shutdownminiute;
	BOOL m_bShutDown;
};

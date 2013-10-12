
// calendarDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <map>
#include "afxdtctl.h"


// CcalendarDlg 对话框
class CcalendarDlg : public CDialog
{
// 构造
public:
	CcalendarDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CALENDAR_DIALOG };

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
	afx_msg void OnBnClickedOk();
	afx_msg void OnMcnSelchangeMonthcalendar1(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL GetStatus();
	BOOL TimeChange;
	bool CreateMatterDir();
	void ShowMatter(CString SelectDay);
	CListBox MatterList;
	std::map<CString, CString> AllMatter;
	CEdit MatterContext;
//	afx_msg void OnLbnSelchangeList1();
	CString SelectDay;
	CMonthCalCtrl DateControl;
	BOOL DeleteFromMatterFile(CString MatterTitle);
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedModifyButton();
};

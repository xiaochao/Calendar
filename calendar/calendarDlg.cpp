
// calendarDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "calendar.h"
#include "calendarDlg.h"
#include "AddMatter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CcalendarDlg 对话框




CcalendarDlg::CcalendarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CcalendarDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	TimeChange = FALSE;
}

void CcalendarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, MatterList);
	DDX_Control(pDX, IDC_MATTER_CONTEXT, MatterContext);
	DDX_Control(pDX, IDC_MONTHCALENDAR1, DateControl);
}

BEGIN_MESSAGE_MAP(CcalendarDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CcalendarDlg::OnBnClickedOk)
	ON_NOTIFY(MCN_SELCHANGE, IDC_MONTHCALENDAR1, &CcalendarDlg::OnMcnSelchangeMonthcalendar1)
//	ON_LBN_SELCHANGE(IDC_LIST1, &CcalendarDlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, &CcalendarDlg::OnBnClickedDeleteButton)
	ON_LBN_SELCHANGE(IDC_LIST1, &CcalendarDlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_MODIFY_BUTTON, &CcalendarDlg::OnBnClickedModifyButton)
END_MESSAGE_MAP()


// CcalendarDlg 消息处理程序

BOOL CcalendarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	if(!CreateMatterDir())
	{
		OnOK();
	}

	CTime cur = CTime::GetCurrentTime();
	SelectDay = cur.Format(_T("%Y-%m-%d"));
	ShowMatter(SelectDay);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CcalendarDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CcalendarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CcalendarDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CAddMatter *add = new CAddMatter;
	add->mod = FALSE;
	add->today = SelectDay;
	add->DoModal();

	ShowMatter(SelectDay);
}

void CcalendarDlg::OnMcnSelchangeMonthcalendar1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);

	CMonthCalCtrl* mcc = (CMonthCalCtrl*)GetDlgItem(IDC_MONTHCALENDAR1);

	CTime time;
	TimeChange = TRUE;

	mcc->GetCurSel( time );
	CStdioFile temp(_T("temp"), CFile::modeCreate|CFile::modeWrite);
	SelectDay = time.Format("%Y-%m-%d");
	temp.WriteString(SelectDay);
	temp.Close();

	ShowMatter(SelectDay);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

BOOL CcalendarDlg::GetStatus()
{
	return TimeChange;
}

bool CcalendarDlg::CreateMatterDir()
{
	CString tempdir(_T("matter"));
	if(!PathIsDirectory(tempdir))
	{
		if(!CreateDirectory(tempdir, NULL))
		{
			CString temp1("create directory ");
			CString temp2("failed, now exists");
			AfxMessageBox(temp1+tempdir+temp2);
			return false;
		}
	}
	return true;
}

void CcalendarDlg::ShowMatter(CString Day)
{
	int times = 0;

	MatterList.ResetContent();
	MatterContext.SetWindowText(_T(""));

	CFileStatus status;
	CString sFile=_T("matter\\")+Day+_T(".txt");
	if (!CFile::GetStatus(sFile,status))
		return;
	
	CStdioFile MatterFile(_T("matter\\")+Day+_T(".txt"), CFile::typeBinary|CFile::modeRead);
	CString Line;
	while(MatterFile.ReadString(Line))
	{
		if(Line.GetLength() == 0)
			break;

		CString title, detail;
		TCHAR sign[] = (_T("\t"));
		AfxExtractSubString(title, Line, 0, *sign);
		AfxExtractSubString(detail, Line, 1,*sign);
		AllMatter[title] = detail;
		
		if(times == 0)
			MatterContext.SetWindowText(detail);
		MatterList.AddString(title);
		times++;
	}
}
//void CcalendarDlg::OnLbnSelchangeList1()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CString buf;
//	MatterList.GetText(MatterList.GetCurSel(), buf);
//	DeleteFromMatterFile(buf);
//}

void CcalendarDlg::OnBnClickedDeleteButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Cursel;
	int site = MatterList.GetCurSel();
	if(site < 0)
	{
		AfxMessageBox(_T("请选择一个要删除的事项"));
		return;
	}
	MatterList.GetText(site, Cursel);
	DeleteFromMatterFile(Cursel);
}

BOOL CcalendarDlg::DeleteFromMatterFile(CString MatterTitle)
{
	CStdioFile temp(_T("matter\\")+SelectDay+_T(".txt"), CFile::typeBinary|CFile::modeRead);
	CStdioFile MatterFile(_T("matter\\")+SelectDay+_T(".txt.temp"), CFile::typeBinary|CFile::modeCreate|CFile::modeWrite);
	WORD unicodeFlag = 0xFEFF;
	MatterFile.Write((void*)&unicodeFlag, sizeof(WORD));
	CString buf;

	while(temp.ReadString(buf))
	{
		CString title;
		TCHAR sign[] = (_T("\t"));
		AfxExtractSubString(title, buf, 0, *sign);
		if(!StrCmp(MatterTitle, title))
			continue;
		MatterFile.WriteString(buf+_T("\n")); 
	}
	temp.Close();
	MatterFile.Close();
	DeleteFile(_T("matter\\")+SelectDay+_T(".txt"));
	MoveFile(_T("matter\\")+SelectDay+_T(".txt.temp"), _T("matter\\")+SelectDay+_T(".txt"));
	ShowMatter(SelectDay);
	
	typedef std::map<CString,CString>::iterator ITER;

	ITER iter=AllMatter.find(MatterTitle);
	AllMatter.erase(iter);

	return TRUE;
}
void CcalendarDlg::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString buf;
	MatterList.GetText(MatterList.GetCurSel(), buf);
	MatterContext.SetWindowText(AllMatter[buf]);
}
void CcalendarDlg::OnBnClickedModifyButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CAddMatter *modify = new CAddMatter;
	CString buf;
	int site = MatterList.GetCurSel();
	if(site < 0)
	{
		AfxMessageBox(_T("请选择一个要修改的事项"));
		return;
	}
	MatterList.GetText(site, buf);
	
	modify->title = buf;
	modify->context = AllMatter[buf];
	modify->today = SelectDay;
	modify->mod = TRUE;

	modify->DoModal();
	ShowMatter(SelectDay);
}

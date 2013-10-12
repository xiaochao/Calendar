// AddMatter.cpp : 实现文件
//

#include "stdafx.h"
#include "calendar.h"
#include "AddMatter.h"
#include "calendarDlg.h"


// CAddMatter 对话框

IMPLEMENT_DYNAMIC(CAddMatter, CDialog)

CAddMatter::CAddMatter(CWnd* pParent /*=NULL*/)
	: CDialog(CAddMatter::IDD, pParent)
{

}

CAddMatter::~CAddMatter()
{
}

void CAddMatter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUT_TITLE, MatterTitle);
	DDX_Control(pDX, IDC_INPUT_CONTENT, MatterContent);
	DDX_Control(pDX, IDOK, SaveButton);
}


BEGIN_MESSAGE_MAP(CAddMatter, CDialog)
	ON_BN_CLICKED(IDOK, &CAddMatter::OnBnClickedOk)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CAddMatter 消息处理程序

void CAddMatter::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(mod)
	{
		CStdioFile temp(_T("matter\\")+today+_T(".txt"), CFile::typeBinary|CFile::modeRead);
		CStdioFile MatterFile(_T("matter\\")+today+_T(".txt.temp"), CFile::typeBinary|CFile::modeCreate|CFile::modeWrite);
		WORD unicodeFlag = 0xFEFF;
		MatterFile.Write((void*)&unicodeFlag, sizeof(WORD));
		CString buf;
		CString NewTitle;
		MatterTitle.GetWindowText(NewTitle);

		while(temp.ReadString(buf))
		{
			CString ti;
			TCHAR sign[] = (_T("\t"));
			AfxExtractSubString(ti, buf, 0, *sign);
			if(!StrCmp(NewTitle, ti))
			{
				CString con;
				MatterContent.GetWindowText(con);
				buf = NewTitle+_T("\t")+con;
			}
			MatterFile.WriteString(buf+_T("\n")); 
		}
		temp.Close();
		MatterFile.Close();
		DeleteFile(_T("matter\\")+today+_T(".txt"));
		MoveFile(_T("matter\\")+today+_T(".txt.temp"), _T("matter\\")+today+_T(".txt"));
		AfxMessageBox(_T("修改成功"));

		return;
	}

	CString content, title;
	MatterContent.GetWindowText(content);
	MatterTitle.GetWindowText(title);

	//判断今天是否有同样的时间存在
	if(IsMatterExists(title))
		return;

	CString buf;
	CFileStatus status;
	
	if (CFile::GetStatus(_T("temp"),status))
	{

		CStdioFile temp(_T("temp"), CFile::modeRead);
		temp.ReadString(buf);
		temp.Close();
		DeleteFile(_T("temp"));
	}
	else
	{
		CTime cur = CTime::GetCurrentTime();
		buf = cur.Format(_T("%Y-%m-%d"));
	}
	
	CStdioFile matter(_T("matter\\")+buf+_T(".txt"), CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate|CFile::typeBinary);
	matter.SeekToEnd();
	WORD unicodeFlag = 0xFEFF;
	CString a = title + _T("\t") + content + _T("\n");
	matter.Write((void*)&unicodeFlag, sizeof(WORD));
	matter.WriteString(a);  
	matter.Close();
	AfxMessageBox(_T("保存成功"));
}


int CAddMatter::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}

void CAddMatter::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	MatterTitle.SetWindowText(title);
	MatterContent.SetWindowText(context);
	if(mod)
	{

		MatterTitle.SetReadOnly(TRUE);
		SaveButton.SetWindowText(_T("修改"));
	}
	else
		SaveButton.SetWindowText(_T("保存"));
}

BOOL CAddMatter::IsMatterExists(CString Matter)
{
	CStdioFile tmp(_T("matter\\")+today+_T(".txt"), CFile::typeBinary|CFile::modeRead);
	CString buf;

	while(tmp.ReadString(buf))
	{
		CString temp_title;
		TCHAR sign[] = (_T("\t"));
		AfxExtractSubString(temp_title, buf, 0, *sign);
		if(!StrCmp(temp_title, Matter))
		{
			AfxMessageBox(_T("今天该事件已经存在，你可以点击编辑进行修改"));
			return TRUE;
		}
	}
	return FALSE;
}
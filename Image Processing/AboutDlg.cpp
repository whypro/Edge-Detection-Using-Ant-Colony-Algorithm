// AboutDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image Processing.h"
#include "AboutDlg.h"
#include "afxdialogex.h"


// CAboutDlg �Ի���

IMPLEMENT_DYNAMIC(CAboutDlg, CDialogEx)

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAboutDlg::IDD, pParent)
{
	messagePointer = 0;
}

BOOL CAboutDlg::OnInitDialog(void)
{
	GetDlgItem(IDC_INFOMATION)->SetWindowText(messageContent[messagePointer]);
	return TRUE;
}


CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_STN_DBLCLK(IDC_FOR_YOU, &CAboutDlg::OnStnDblclickForYou)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
	ON_STN_DBLCLK(IDC_LOGO, &CAboutDlg::OnStnDblclickLogo)
	ON_STN_CLICKED(IDC_INFOMATION, &CAboutDlg::OnStnClickedInfomation)
END_MESSAGE_MAP()


// CAboutDlg ��Ϣ�������

void CAboutDlg::OnStnDblclickForYou()
{
	GetDlgItem(IDC_FOR_YOU_PROMT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_FOR_YOU_PASSWORD)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_FOR_YOU_PASSWORD)->SetFocus();
}

void CAboutDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
	CString strPass("");
	GetDlgItem(IDC_FOR_YOU_PASSWORD)->GetWindowText(strPass);
	if (strPass == _T("micycalvi"))
	{
		AfxMessageBox(_T("������գ��"));
		AfxMessageBox(_T("�����ڸ質"));
		AfxMessageBox(_T("For you, R..."));
	}
}

void CAboutDlg::OnStnDblclickLogo()
{

}

void CAboutDlg::OnStnClickedInfomation()
{
	if (++messagePointer == messageCount)
	{
		messagePointer = 0;
	}
	GetDlgItem(IDC_INFOMATION)->SetWindowText(messageContent[messagePointer]);
}


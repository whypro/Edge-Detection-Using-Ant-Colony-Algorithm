#pragma once


// CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAboutDlg)

public:
	CAboutDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnDblclickForYou();
	afx_msg void OnBnClickedOk();
	afx_msg void OnStnDblclickLogo();
	afx_msg void OnStnClickedInfomation();
private:
	int messagePointer;
public:
	BOOL OnInitDialog(void);
};

static const int messageCount = 6;
static const CString messageContent[messageCount] = {
	_T("Don't Click Here..."),
	_T("Whoops, If you have clicked..."),
	_T("The program's developed by Haoyu Wang."),
	_T("(C) 2012 Whypro Studio"),
	_T("E-mail: whypro@live.cn"),
	_T("April 25, 2012")
};


// Image ProcessingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image Processing.h"
#include "Image ProcessingDlg.h"
#include "AfxDialogEX.h"
#include "AntColony.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "cxcore.h"
#include "highgui.h"

#include <cstring>

// CImageProcessingDlg �Ի���
HANDLE g_hEventACCreated = NULL;	
HANDLE g_hEventProgressEnded = NULL;
bool g_isACRunning = false;
bool g_isProgressRunning = false;


CImageProcessingDlg::CImageProcessingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageProcessingDlg::IDD, pParent)
	, m_nCanny_P1(150)
	, m_nCanny_P2(50)
	, m_nCanny_P3(3)
	, m_nLaplace_P1(3)
	, m_nSobel_P1(1)
	, m_nSobel_P2(1)
	, m_nSobel_P3(3)
	, m_nAntNumber(2000)
	, m_nAntIterNumber(1275)
	, m_fAntEvaporation(0.1)
	, m_fThresholdVal(0.3)
	, m_fAlpha(1)
	, m_fBeta(1)
	, m_fGamma(0)
	, m_nPixelStepA(3)
	, m_nPixelStepB(3)
	, m_nAntMemLength(10)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageProcessingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CANNY_P1, m_nCanny_P1);
	DDV_MinMaxInt(pDX, m_nCanny_P1, 0, 1000);
	DDX_Text(pDX, IDC_CANNY_P2, m_nCanny_P2);
	DDV_MinMaxInt(pDX, m_nCanny_P2, 0, 1000);
	DDX_Text(pDX, IDC_CANNY_P3, m_nCanny_P3);
	DDV_MinMaxInt(pDX, m_nCanny_P3, 0, 1000);
	DDX_Text(pDX, IDC_LAPLACE_P1, m_nLaplace_P1);
	DDX_Text(pDX, IDC_SOBEL_P1, m_nSobel_P1);
	DDX_Text(pDX, IDC_SOBEL_P2, m_nSobel_P2);
	DDX_Text(pDX, IDC_SOBEL_P3, m_nSobel_P3);
	DDX_Text(pDX, IDC_ANT_NUMBER, m_nAntNumber);
	DDX_Text(pDX, IDC_ANT_ITER_NUM, m_nAntIterNumber);
	DDX_Text(pDX, IDC_ANT_EVAPORATION, m_fAntEvaporation);
	DDV_MinMaxInt(pDX, m_fAntEvaporation, 0, 1);
	DDX_Text(pDX, IDC_THRESHOLD_VAL, m_fThresholdVal);
	DDV_MinMaxInt(pDX, m_fThresholdVal, 0, 1);
	DDX_Text(pDX, IDC_ANT_ALPHA, m_fAlpha);
	DDV_MinMaxDouble(pDX, m_fAlpha, 0, 10);
	DDX_Text(pDX, IDC_ANT_BETA, m_fBeta);
	DDV_MinMaxDouble(pDX, m_fBeta, 0, 10);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Text(pDX, IDC_PIXEL_STEP_A, m_nPixelStepA);
	DDX_Text(pDX, IDC_PIXEL_STEP_B, m_nPixelStepB);
	DDX_Text(pDX, IDC_ANT_GAMMA, m_fGamma);
	DDX_Text(pDX, IDC_ANT_MEM_LENGTH, m_nAntMemLength);
}

BEGIN_MESSAGE_MAP(CImageProcessingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CImageProcessingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_GRAY, &CImageProcessingDlg::OnBnClickedGray)
	ON_BN_CLICKED(IDC_BACK, &CImageProcessingDlg::OnBnClickedBack)
	ON_BN_CLICKED(IDC_CANNY, &CImageProcessingDlg::OnBnClickedCanny)
	ON_BN_CLICKED(IDC_LAPLACE, &CImageProcessingDlg::OnBnClickedLaplace)
	ON_BN_CLICKED(IDC_LOG, &CImageProcessingDlg::OnBnClickedLog)
	ON_BN_CLICKED(IDC_BROWSE, &CImageProcessingDlg::OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_RESET, &CImageProcessingDlg::OnBnClickedReset)
	ON_BN_CLICKED(IDC_SAVE, &CImageProcessingDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_OPEN, &CImageProcessingDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_ABOUT, &CImageProcessingDlg::OnBnClickedAbout)
	ON_BN_CLICKED(IDC_SOBEL, &CImageProcessingDlg::OnBnClickedSobel)
	ON_BN_CLICKED(IDC_SMOOTH, &CImageProcessingDlg::OnBnClickedSmooth)
	ON_BN_CLICKED(IDC_ANTCOLONY, &CImageProcessingDlg::OnBnClickedAntColony)
	ON_BN_CLICKED(IDC_ANTCOLONY_AUTOP, &CImageProcessingDlg::OnBnClickedAntcolonyAutop)
	ON_BN_CLICKED(IDC_PREPROC, &CImageProcessingDlg::OnBnClickedPreproc)
	ON_BN_CLICKED(IDC_THRESHOLD, &CImageProcessingDlg::OnBnClickedThreshold)
	ON_STN_DBLCLK(IDC_IMAGE_FIELD, &CImageProcessingDlg::OnStnDblclickImageField)
	ON_NOTIFY(UDN_DELTAPOS, IDC_THRESHOLD_VAL_SPIN, &CImageProcessingDlg::OnDeltaposThresholdValSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ANT_NUMBER_SPIN, &CImageProcessingDlg::OnDeltaposAntNumberSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ANT_ITER_NUM_SPIN, &CImageProcessingDlg::OnDeltaposAntIterNumSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PIXEL_STEPA_SPIN, &CImageProcessingDlg::OnDeltaposPixelStepaSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PIXEL_STEPB_SPIN, &CImageProcessingDlg::OnDeltaposPixelStepbSpin)
	//ON_WM_VSCROLL()
	ON_NOTIFY(UDN_DELTAPOS, IDC_ANT_ALPHA_SPIN, &CImageProcessingDlg::OnDeltaposAntAlphaSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ANT_BETA_SPIN, &CImageProcessingDlg::OnDeltaposAntBetaSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ANT_EVAPORATION_SPIN, &CImageProcessingDlg::OnDeltaposAntEvaporationSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ANT_GAMMA_SPIN, &CImageProcessingDlg::OnDeltaposAntGammaSpin)
	ON_WM_DESTROY()
//	ON_CBN_SELCHANGE(IDC_PREPROC_TYPE, &CImageProcessingDlg::OnCbnSelchangePreprocType)
ON_CBN_SELCHANGE(IDC_ANT_DISTRIBUTE_TYPE, &CImageProcessingDlg::OnCbnSelchangeAntDistributeType)
ON_BN_CLICKED(IDC_THIN, &CImageProcessingDlg::OnBnClickedThin)
ON_NOTIFY(UDN_DELTAPOS, IDC_ANT_MEM_LENGTH_SPIN, &CImageProcessingDlg::OnDeltaposAntMemLengthSpin)
END_MESSAGE_MAP()

// CImageProcessingDlg ��Ϣ�������

BOOL CImageProcessingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	pWImage = new CWImage();
	pAntColony = NULL;
	pAntColonyThread = NULL;
	pProgressThread = NULL;

	const int grayTypeCount = 7;
	std::string grayTypeStr[grayTypeCount] = {
		"R ����",
		"G ����",
		"B ����",
		"���ֵ",
		"ƽ��ֵ",
		"��Ȩƽ��ֵ",
		"OpenCV"
	};
	const int preProcTypeCount = 3;
	std::string preProcTypeStr[preProcTypeCount] = {
		"�ݶ�����",
		"Roberts ����",
		"Sobel ����"
	};
	const int antDistributeTypeCount = 3;
	std::string antDistributeTypeStr[antDistributeTypeCount] = {
		"����ֲ�",
		"���ȷֲ�",
		"���ֲܷ�"
	};
	const int antMoveTypeCount = 3;
	std::string antMoveTypeStr[antMoveTypeCount] = {
		"��ʽһ",
		"��ʽ��",
		"��ʽ��"
	};
	const int thinningTypeCount = 4;
	std::string thinningType[thinningTypeCount] = {
		"Hilditch",
		"Pavlidis",
		"Rosenfeld",
		"Skeleton"
	};
	// �ҶȻ�ѡ��
	CComboBox *pCB = (CComboBox *)GetDlgItem(IDC_GRAY_TYPE);
	for (int i = 0; i < grayTypeCount; ++i)
	{
		pCB->AddString(grayTypeStr[i].c_str());
	}
	pCB->SetCurSel(6);	// �ҶȻ� Ĭ��ѡ��
	// Ԥ����ѡ��
	pCB = (CComboBox *)GetDlgItem(IDC_PREPROC_TYPE);
	for (int i = 0; i < preProcTypeCount; ++i)
	{
		pCB->AddString(preProcTypeStr[i].c_str());
	}
	pCB->SetCurSel(0);	// Ԥ���� Ĭ��ѡ��
	// ���ϳ�ʼ�ֲ�ѡ��
	pCB = (CComboBox *)GetDlgItem(IDC_ANT_DISTRIBUTE_TYPE);
	for (int i = 0; i < antDistributeTypeCount; ++i)
	{
		pCB->AddString(antDistributeTypeStr[i].c_str());
	}
	pCB->SetCurSel(2);	// ���ϳ�ʼ�ֲ� Ĭ��ѡ��
	// �����ƶ���ʽѡ��
	pCB = (CComboBox *)GetDlgItem(IDC_MOVE_TYPE);
	for (int i = 0; i < antMoveTypeCount; ++i)
	{
		pCB->AddString(antMoveTypeStr[i].c_str());
	}
	pCB->SetCurSel(0);	// �����ƶ���ʽ Ĭ��ѡ��
	// ϸ���㷨
	pCB = (CComboBox *)GetDlgItem(IDC_THINNING_TYPE);
	for (int i = 0; i < thinningTypeCount; ++i)
	{
		pCB->AddString(thinningType[i].c_str());
	}
	pCB->SetCurSel(1);	// ϸ���㷨 Ĭ��ѡ��
	SetButtonState();
	//((CSpinButtonCtrl *)GetDlgItem(IDC_THRESHOLD_VAL_SPIN))->SetRange(0, 100);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CImageProcessingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CImageProcessingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HBRUSH CImageProcessingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWImage->Size() > 0)
	{
		DrawImageToHDC(pWImage->GetCurImage(), IDC_IMAGE_FIELD);
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CImageProcessingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CImageProcessingDlg::OnDropFiles(HDROP hDropInfo)
{
	char imagePath[_MAX_PATH] = { 0 };
	if (DragQueryFile(hDropInfo, 0, imagePath, _MAX_PATH))
	{
		GetDlgItem(IDC_IMAGE_PATH)->SetWindowText(imagePath);
		DragFinish(hDropInfo);
		OnBnClickedOpen();
	}

	CDialogEx::OnDropFiles(hDropInfo);
}

BOOL CImageProcessingDlg::DrawImageToHDC(IplImage *img, UINT ID) 
{
	if (NULL == img) 
	{
		return FALSE;
	}
	CWnd *pWnd = GetDlgItem(ID);
	CDC *pDC = pWnd->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	pWnd->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(img, img->nChannels);
	cimg.DrawToHDC(hDC,&rect);
	return TRUE;
}

void CImageProcessingDlg::OnBnClickedOk()
{

}

void CImageProcessingDlg::OnBnClickedBrowse()
{
	// �ļ���չ��������
	LPCTSTR lpszFilter = _T("�����ļ� (*.*)|*.*|"
		"Windowsλͼ�ļ� (*.bmp; *.dib)|*.bmp; *.dib|"
		"JPEG�ļ� (*.jpeg; *.jpg; *.jpe)|*.jpeg; *.jpg; *.jpe|"
		"��Яʽ����ͼƬ (*.png)|*.png|"
		"��Яʽͼ���ʽ (*.pbm; *.pgm; *.ppm)|*.pbm; *.pgm; *.ppm)|"
		"Sun rasters (*.sr; *.ras)|*.sr; *.ras|"
		"TIFF�ļ� (*.tiff; *.tif)|*.tiff; *.tif|"
		"OpenEXR HDRͼƬ (*.exr)|*.exr|"
		"JPEG 2000ͼƬ (*.jp2)|*.jp2||");
	CFileDialog openFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, lpszFilter);
	if(openFileDlg.DoModal() == IDOK)
	{
		CString strPath = openFileDlg.GetPathName(); // ȫ·��
		GetDlgItem(IDC_IMAGE_PATH)->SetWindowText(strPath);
		OnBnClickedOpen();
	}
}

void CImageProcessingDlg::OnBnClickedOpen()
{
	CString strPath;
	GetDlgItem(IDC_IMAGE_PATH)->GetWindowText(strPath);
	std::string imagePath(strPath.GetBuffer(strPath.GetLength()));
	if (pWImage->AddImage(imagePath) == false)
	{
		//GetDlgItem(IDC_IMAGE_PATH)->SetWindowText(_T("��֧�ֵ��ļ���ʽ��"));
		AfxMessageBox(_T("��֧�ֵ��ļ���ʽ��"));
		return;

	}
	pWImage->Clear();
	pWImage->AddImage(imagePath);
	m_ctrlProgress.SetPos(0);
	DrawImageToHDC(pWImage->GetCurImage(), IDC_IMAGE_FIELD);
	SetButtonState();
}

void CImageProcessingDlg::OnBnClickedSave()
{
	// �ļ���չ��������
	LPCTSTR lpszFilter = _T("Windowsλͼ�ļ� (*.bmp; *.dib)|*.bmp; *.dib|"
		"JPEG�ļ� (*.jpeg; *.jpg; *.jpe)|*.jpeg; *.jpg; *.jpe|"
		"��Яʽ����ͼƬ (*.png)|*.png|"
		"��Яʽͼ���ʽ (*.pbm; *.pgm; *.ppm)|*.pbm; *.pgm; *.ppm)|"
		"Sun rasters (*.sr; *.ras)|*.sr; *.ras|"
		"TIFF�ļ� (*.tiff; *.tif)|*.tiff; *.tif|"
		"OpenEXR HDRͼƬ (*.exr)|*.exr|"
		"JPEG 2000ͼƬ (*.jp2)|*.jp2||");
	CFileDialog openFileDlg(FALSE, _T("*.*"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilter);
	if(openFileDlg.DoModal() == IDOK)
	{
		CString strPath = openFileDlg.GetPathName(); // ȫ·��
		std::string imagePath(strPath.GetBuffer(strPath.GetLength()));
		if (pWImage->SaveCurImage(imagePath) == false)
		{
			AfxMessageBox(_T("�����ļ�ʧ�ܣ�"));	
			return;
		}
		// AfxMessageBox(_T("�ѱ��棡"));
	}

}

void CImageProcessingDlg::OnBnClickedAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CImageProcessingDlg::OnBnClickedGray()
{
	int nIndex = ((CComboBox *)GetDlgItem(IDC_GRAY_TYPE))->GetCurSel();
	pWImage->GrayEX(static_cast<CWImage::GType>(nIndex));
	DrawImageToHDC(pWImage->GetCurImage(), IDC_IMAGE_FIELD);
	SetButtonState();
}

void CImageProcessingDlg::OnBnClickedPreproc()
{
	int nIndex = ((CComboBox *)GetDlgItem(IDC_PREPROC_TYPE))->GetCurSel();
	bool flag = true;
	switch (nIndex)
	{
	case 0:	// �ݶ�����
		pWImage->Grad();
		break;
	case 1:	// Roberts ����
		pWImage->Roberts();
		break;
	case 2:	// Sobel ����
		pWImage->Sobel();
		break;
	default:
		flag = false;
		break;
	}
	if (flag)
	{
		DrawImageToHDC(pWImage->GetCurImage(), IDC_IMAGE_FIELD);
		SetButtonState();
	}
}

void CImageProcessingDlg::OnBnClickedBack()
{
	pWImage->DropCurImage();
	DrawImageToHDC(pWImage->GetCurImage(), IDC_IMAGE_FIELD);
	SetButtonState();
}

void CImageProcessingDlg::OnBnClickedReset()
{
	pWImage->Reset();
	DrawImageToHDC(pWImage->GetCurImage(), IDC_IMAGE_FIELD);
	SetButtonState();
}

void CImageProcessingDlg::OnBnClickedCanny()
{
	UpdateData(TRUE);
	pWImage->Canny(m_nCanny_P1, m_nCanny_P2, m_nCanny_P3);
	DrawImageToHDC(pWImage->GetCurImage(), IDC_IMAGE_FIELD);
	SetButtonState();
}

void CImageProcessingDlg::OnBnClickedSobel()
{
	UpdateData(TRUE);
	if (m_nSobel_P3 != 1 && m_nSobel_P3 != 3 &&
		m_nSobel_P3 != 5 && m_nSobel_P3 != 7) 
	{
		AfxMessageBox(_T("Sobel���ӵĵ�������������Ϊ��1, 3, 5, 7"));
		m_nSobel_P3 = 3;
		UpdateData(FALSE);
		return;
	}
	pWImage->Sobel(m_nSobel_P1, m_nSobel_P2, m_nSobel_P3);
	DrawImageToHDC(pWImage->GetCurImage(), IDC_IMAGE_FIELD);
	SetButtonState();
}

void CImageProcessingDlg::OnBnClickedLaplace()
{
	UpdateData(TRUE);
	if (m_nLaplace_P1 != 1 && m_nLaplace_P1 != 3 &&
		m_nLaplace_P1 != 5 && m_nLaplace_P1 != 7) 
	{
		AfxMessageBox(_T("Laplace���ӵĲ�������Ϊ��1, 3, 5, 7"));
		m_nLaplace_P1 = 3;
		UpdateData(FALSE);
		return;
	}
	pWImage->Laplace(m_nLaplace_P1);
	DrawImageToHDC(pWImage->GetCurImage(), IDC_IMAGE_FIELD);
	SetButtonState();

}

void CImageProcessingDlg::OnBnClickedLog()
{
}

void CImageProcessingDlg::OnBnClickedSmooth()
{
	pWImage->Smooth();
	DrawImageToHDC(pWImage->GetCurImage(), IDC_IMAGE_FIELD);
	SetButtonState();
}

void CImageProcessingDlg::OnBnClickedAntColony()
{
	// �㷨�������У���ֹ
	if (g_isACRunning || g_isProgressRunning) 
	{
		// ����ȫ�ķ���
		// ��Ҫ���õķ������ԸĽ�������
		TerminateThread(pAntColonyThread->m_hThread, -1);
		TerminateThread(pProgressThread->m_hThread, -1);
		g_isACRunning = false;
		g_isProgressRunning = false;
		SetButtonState();
	}
	else
	{
		UpdateData(TRUE);
		if (m_fAntEvaporation < 0 || m_fAntEvaporation > 1 || m_fThresholdVal < 0 || m_fThresholdVal > 1)
		{
			AfxMessageBox(_T("����ϵ������ֵ����Ϊ 0 ~ 1 ֮���ʵ��"));
			return;
		}
		g_isACRunning = true;
		g_isProgressRunning = true;
		SetButtonState();
		g_hEventACCreated = CreateEvent(NULL, FALSE, FALSE, "Ant Colony Created Event");
		g_hEventProgressEnded = CreateEvent(NULL, FALSE, FALSE, "ProgressCtrl Ended Event");	
		pProgressThread = AfxBeginThread(ProgressThreadFunc, this);
		pAntColonyThread = AfxBeginThread(AntColonyThreadFunc, this);
	}
}

void CImageProcessingDlg::OnBnClickedThreshold()
{
	UpdateData(TRUE);
	pWImage->Thresholding(m_fThresholdVal);
	DrawImageToHDC(pWImage->GetCurImage(), IDC_IMAGE_FIELD);
	SetButtonState();

}

void CImageProcessingDlg::OnBnClickedAntcolonyAutop()
{
	// �Զ�������������������㷽ʽ��Ľ�
	int antNumber = ceil((double) pWImage->GetCurImage()->height / 3) * ceil((double) pWImage->GetCurImage()->width / 3);
	int itNumber = 1000;//(pWImage->GetCurImage()->height * pWImage->GetCurImage()->width) / antNumber;
	m_nAntNumber = antNumber;
	m_nAntIterNumber = itNumber;
	UpdateData(FALSE);
	CButton *pCheck = (CButton *)GetDlgItem(IDC_ANTCOLONY_AUTOP);

	if (pCheck->GetCheck() == 0)
	{
		GetDlgItem(IDC_ANT_NUMBER)->EnableWindow(TRUE);
		GetDlgItem(IDC_ANT_ITER_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_ANT_EVAPORATION)->EnableWindow(TRUE);
		GetDlgItem(IDC_THRESHOLD_VAL)->EnableWindow(TRUE);
	}
	else 
	{
		GetDlgItem(IDC_ANT_NUMBER)->EnableWindow(FALSE);
		GetDlgItem(IDC_ANT_ITER_NUM)->EnableWindow(FALSE);
		GetDlgItem(IDC_ANT_EVAPORATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_THRESHOLD_VAL)->EnableWindow(FALSE);
	}
}

void CImageProcessingDlg::SetButtonState(void)
{
	// ����㷨����ִ�У�ֱ�ӽ������¿ؼ�
	if (g_isACRunning || g_isProgressRunning)
	{
		GetDlgItem(IDC_BROWSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_GRAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
		GetDlgItem(IDC_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_CANNY)->EnableWindow(FALSE);
		GetDlgItem(IDC_LAPLACE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SOBEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_SMOOTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_PREPROC)->EnableWindow(FALSE);
		//GetDlgItem(IDC_ANTCOLONY)->EnableWindow(FALSE);
		GetDlgItem(IDC_THRESHOLD)->EnableWindow(FALSE);
		GetDlgItem(IDC_ANTCOLONY_AUTOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_ANTCOLONY)->SetWindowText(_T("��ֹ(&X)"));
	}
	else	// ����㷨û��ִ�У��ٽ��������ж�
	{
		GetDlgItem(IDC_ANTCOLONY)->SetWindowText(_T("��Ⱥ�㷨(&B)"));
		GetDlgItem(IDC_BROWSE)->EnableWindow(TRUE);
		if (pWImage->Size() <= 0)
		{
			GetDlgItem(IDC_SAVE)->EnableWindow(FALSE);
			GetDlgItem(IDC_GRAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
			GetDlgItem(IDC_RESET)->EnableWindow(FALSE);
			GetDlgItem(IDC_CANNY)->EnableWindow(FALSE);
			GetDlgItem(IDC_LAPLACE)->EnableWindow(FALSE);
			GetDlgItem(IDC_SOBEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_SMOOTH)->EnableWindow(FALSE);
			GetDlgItem(IDC_PREPROC)->EnableWindow(FALSE);
			GetDlgItem(IDC_ANTCOLONY)->EnableWindow(FALSE);
			GetDlgItem(IDC_THRESHOLD)->EnableWindow(FALSE);
			GetDlgItem(IDC_ANTCOLONY_AUTOP)->EnableWindow(FALSE);
		}
		else	
		{
			GetDlgItem(IDC_SAVE)->EnableWindow(TRUE);
			GetDlgItem(IDC_ANTCOLONY_AUTOP)->EnableWindow(TRUE);
			if (pWImage->Size() == 1)
			{
				GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
				GetDlgItem(IDC_RESET)->EnableWindow(FALSE);
			}
			else 
			{
				GetDlgItem(IDC_BACK)->EnableWindow(TRUE);
				GetDlgItem(IDC_RESET)->EnableWindow(TRUE);
			}
			char *mode = pWImage->GetCurImage()->colorModel;
			if (!strncmp(mode, "RGB\0", 4)) 
			{
				GetDlgItem(IDC_GRAY)->EnableWindow(TRUE);
				GetDlgItem(IDC_CANNY)->EnableWindow(FALSE);
				GetDlgItem(IDC_LAPLACE)->EnableWindow(FALSE);
				GetDlgItem(IDC_SOBEL)->EnableWindow(FALSE);
				GetDlgItem(IDC_SMOOTH)->EnableWindow(FALSE);
				GetDlgItem(IDC_PREPROC)->EnableWindow(FALSE);
				GetDlgItem(IDC_THRESHOLD)->EnableWindow(FALSE);
				GetDlgItem(IDC_ANTCOLONY)->EnableWindow(FALSE);
				
			}
			else if (!strncmp(mode, "GRAY", 4)) 
			{
				GetDlgItem(IDC_GRAY)->EnableWindow(FALSE);
				GetDlgItem(IDC_CANNY)->EnableWindow(TRUE);
				GetDlgItem(IDC_LAPLACE)->EnableWindow(TRUE);
				GetDlgItem(IDC_SOBEL)->EnableWindow(TRUE);
				GetDlgItem(IDC_SMOOTH)->EnableWindow(TRUE);
				GetDlgItem(IDC_PREPROC)->EnableWindow(TRUE);
				GetDlgItem(IDC_THRESHOLD)->EnableWindow(TRUE);
				GetDlgItem(IDC_ANTCOLONY)->EnableWindow(TRUE);
			}
		}
	}
}

// �������ؼ��̺߳���
UINT ProgressThreadFunc(LPVOID lpParam)
{
	// ��֤��ǰֻ��һ���߳�ִ��
	CImageProcessingDlg *pThis = (CImageProcessingDlg *) lpParam;
	if (pThis == NULL)
	{
		return -1;
	}
	CProgressCtrl *pCtrl = &(pThis->m_ctrlProgress);
	
	int nCur = 0;		// ��ǰ������λ��
	int nMax = 1;		// 
	int nPre = 0;		// ǰһ������λ��
	int nWait;
	//nWait = WaitForSingleObject(g_hMutexACCreated, INFINITE);	// �ȴ����������źţ�P
	nWait = WaitForSingleObject(g_hEventACCreated, INFINITE);
	if (nWait == WAIT_OBJECT_0)
	{
		// ��ȡ����������
		nMax = pThis->pAntColony->GetMaxIterationNumber();
	}
	else // �������ź���
	{
		return -2;
	}
	pCtrl->SetRange(0, nMax);
	pCtrl->SetPos(0);
	//ReleaseMutex(g_hMutexACCreated);								// �ͷ��ź�����V
	while (nCur < nMax)
	{
		nCur = pThis->pAntColony->GetCurIterationNumber();
		if (nCur != nPre)
		{
			pCtrl->SetPos(nCur);
			nPre = nCur;
		}
		Sleep(500);
	}
	pCtrl->SetPos(nMax);
	SetEvent(g_hEventProgressEnded);
	//ReleaseMutex(g_hMutexProgressEnded);
	
	g_isProgressRunning = false;
	pThis->SetButtonState();
	return 0;
}

// ��Ⱥ�㷨�̺߳���
UINT AntColonyThreadFunc(LPVOID lpParam)
{
	CImageProcessingDlg *pThis = (CImageProcessingDlg *) lpParam;		
	if (pThis == NULL)
	{
		return -1;
	}

	// ��ʼ��ʱ
	clock_t cBegin, cEnd;  
	cBegin = clock();  
	pThis->pAntColony = new CAntColony(
		pThis->pWImage->GetCurImage(), 
		pThis->m_nAntNumber,
		pThis->m_nAntIterNumber,
		pThis->m_nAntMemLength,
		pThis->m_nPixelStepA,
		pThis->m_nPixelStepB,
		pThis->m_fAlpha,
		pThis->m_fBeta,
		pThis->m_fGamma,
		pThis->m_fAntEvaporation);
	SetEvent(g_hEventACCreated);
	int nDisType = ((CComboBox *)pThis->GetDlgItem(IDC_ANT_DISTRIBUTE_TYPE))->GetCurSel();
	pThis->pAntColony->Init(static_cast<CAntColony::DistributeType>(nDisType));
	pThis->pAntColony->Move();
	IplImage *pImage2 = cvCreateImage(cvGetSize(pThis->pWImage->GetCurImage()),IPL_DEPTH_8U,1);
	pThis->pAntColony->GetGlobalPheromoneImage(pImage2);
	int nWait = WaitForSingleObject(g_hEventProgressEnded, INFINITE);
	if (nWait == WAIT_OBJECT_0)
	{
		pThis->pWImage->AddImage(pImage2);
		delete pThis->pAntColony;
		pThis->pAntColony = NULL;
	}
	cEnd = clock();
	CString timeDiff;
	timeDiff.Format(_T("��ʱ %.3f ��"), double (cEnd - cBegin) / 1000);
	pThis->GetDlgItem(IDC_IMAGE_PATH)->SetWindowText(timeDiff);
	g_isACRunning = false;
	pThis->SetButtonState();
	return 0;
}


void CImageProcessingDlg::OnStnDblclickImageField()
{
	// �´�����ʾͼƬ
	IplImage *pImage = pWImage->GetCurImage();
	if (!pImage) 
	{
		return;
	}
	cvNamedWindow("Image", CV_WINDOW_AUTOSIZE); //���崰��
	cvShowImage("Image", pImage);
}


void CImageProcessingDlg::OnDeltaposThresholdValSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == 1)
	{
		m_fThresholdVal += 0.01;
		if (m_fThresholdVal > 1)
		{
			m_fThresholdVal = 1;
		}
	}
	else if (pNMUpDown->iDelta == -1)
	{
		m_fThresholdVal -= 0.01;
		if (m_fThresholdVal < 0)
		{
			m_fThresholdVal = 0;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CImageProcessingDlg::OnDeltaposAntNumberSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == 1)
	{
		m_nAntNumber += 10;
	}
	else if (pNMUpDown->iDelta == -1)
	{
		m_nAntNumber -= 10;
		if (m_nAntNumber < 0)
		{
			m_nAntNumber = 0;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CImageProcessingDlg::OnDeltaposAntIterNumSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == 1)
	{
		m_nAntIterNumber += 10;
	}
	else if (pNMUpDown->iDelta == -1)
	{
		m_nAntIterNumber -= 10;
		if (m_nAntIterNumber < 0)
		{
			m_nAntIterNumber = 0;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CImageProcessingDlg::OnDeltaposPixelStepaSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == 1)
	{
		m_nPixelStepA += 1;
	}
	else if (pNMUpDown->iDelta == -1)
	{
		m_nPixelStepA -= 1;
		if (m_nPixelStepA < 0)
		{
			m_nPixelStepA = 0;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CImageProcessingDlg::OnDeltaposPixelStepbSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == 1)
	{
		m_nPixelStepB += 1;
	}
	else if (pNMUpDown->iDelta == -1)
	{
		m_nPixelStepB -= 1;
		if (m_nPixelStepB < 0)
		{
			m_nPixelStepB = 0;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CImageProcessingDlg::OnDeltaposAntAlphaSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == 1)
	{
		m_fAlpha += 0.1;
	}
	else if (pNMUpDown->iDelta == -1)
	{
		m_fAlpha -= 0.1;
		if (m_fAlpha < 0)
		{
			m_fAlpha = 0;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CImageProcessingDlg::OnDeltaposAntBetaSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == 1)
	{
		m_fBeta += 0.1;
	}
	else if (pNMUpDown->iDelta == -1)
	{
		m_fBeta -= 0.1;
		if (m_fBeta < 0)
		{
			m_fBeta = 0;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CImageProcessingDlg::OnDeltaposAntEvaporationSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == 1)
	{
		m_fAntEvaporation += 0.1;
		if (m_fAntEvaporation > 1)
		{
			m_fAntEvaporation = 1;
		}
	}
	else if (pNMUpDown->iDelta == -1)
	{
		m_fAntEvaporation -= 0.1;
		if (m_fAntEvaporation < 0)
		{
			m_fAntEvaporation = 0;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CImageProcessingDlg::OnDeltaposAntGammaSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == 1)
	{
		m_fGamma += 0.1;
	}
	else if (pNMUpDown->iDelta == -1)
	{
		m_fGamma -= 0.1;
		if (m_fGamma < 0)
		{
			m_fGamma = 0;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CImageProcessingDlg::OnDeltaposAntMemLengthSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == 1)
	{
		m_nAntMemLength += 1;
	}
	else if (pNMUpDown->iDelta == -1)
	{
		m_nAntMemLength -= 1;
		if (m_nAntMemLength < 0)
		{
			m_nAntMemLength = 0;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CImageProcessingDlg::OnDestroy()
{
	delete pWImage;
	CDialogEx::OnDestroy();
}

void CImageProcessingDlg::OnCbnSelchangeAntDistributeType()
{
	IplImage *pImage = pWImage->GetCurImage();
	if (pImage == NULL)
	{
		return;
	}
	UpdateData(TRUE);
	int nIndex = ((CComboBox *)GetDlgItem(IDC_ANT_DISTRIBUTE_TYPE))->GetCurSel();
	switch (static_cast<CAntColony::DistributeType>(nIndex))
	{
	case CAntColony::RANDOM_DIS:
		m_nAntNumber = sqrt(double (pImage->width * pImage->height));
		GetDlgItem(IDC_ANT_NUMBER)->EnableWindow(TRUE);
		GetDlgItem(IDC_PIXEL_STEP_A)->EnableWindow(FALSE);
		GetDlgItem(IDC_PIXEL_STEP_B)->EnableWindow(FALSE);
		GetDlgItem(IDC_ANT_ITER_NUM)->EnableWindow(TRUE);
		break;
	case CAntColony::UNIFORM_DIS:
		m_nAntNumber = ceil(double (pImage->width) / m_nPixelStepA) * ceil(double (pImage->height) / m_nPixelStepB);
		m_nAntIterNumber = 255 * sqrt(double(m_nPixelStepA * m_nPixelStepB));
		GetDlgItem(IDC_ANT_NUMBER)->EnableWindow(FALSE);
		GetDlgItem(IDC_PIXEL_STEP_A)->EnableWindow(TRUE);
		GetDlgItem(IDC_PIXEL_STEP_B)->EnableWindow(TRUE);
		GetDlgItem(IDC_ANT_ITER_NUM)->EnableWindow(TRUE);
		break;
	case CAntColony::SMART_DIS:
		m_nAntNumber = ceil(double (pImage->width) / m_nPixelStepA) * ceil(double (pImage->height) / m_nPixelStepB);
		m_nAntIterNumber = 255 * sqrt(double(m_nPixelStepA * m_nPixelStepB));
		GetDlgItem(IDC_ANT_NUMBER)->EnableWindow(FALSE);
		GetDlgItem(IDC_PIXEL_STEP_A)->EnableWindow(TRUE);
		GetDlgItem(IDC_PIXEL_STEP_B)->EnableWindow(TRUE);
		GetDlgItem(IDC_ANT_ITER_NUM)->EnableWindow(TRUE);
		break;
	default:
		break;
	}
	UpdateData(FALSE);
}


void CImageProcessingDlg::OnBnClickedThin()
{
	int nIndex = ((CComboBox *)GetDlgItem(IDC_THINNING_TYPE))->GetCurSel();
	pWImage->Thin(static_cast<CWImage::ThinType>(nIndex));
	DrawImageToHDC(pWImage->GetCurImage(), IDC_IMAGE_FIELD);
	SetButtonState();
}



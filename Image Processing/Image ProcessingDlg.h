// Image ProcessingDlg.h : 头文件
//

#pragma once
#include "cv.h"
#include "highgui.h"
#include "WImage.h"
#include "afxcmn.h"

class CAntColony;
// CImageProcessingDlg 对话框
class CImageProcessingDlg : public CDialogEx
{
// 构造
public:
	CImageProcessingDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_IMAGEPROCESSING_DIALOG };

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
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	DECLARE_MESSAGE_MAP()
private:
	CWImage *pWImage;
public:
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedAbout();
	afx_msg void OnBnClickedGray();
	afx_msg void OnBnClickedBack();
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedOk();
	
	afx_msg void OnBnClickedCanny();
	afx_msg void OnBnClickedSobel();
	afx_msg void OnBnClickedLaplace();
	afx_msg void OnBnClickedLog();
	afx_msg void OnBnClickedSmooth();
	afx_msg void OnBnClickedAntColony();
	afx_msg void OnBnClickedAntcolonyAutop();
	
	BOOL DrawImageToHDC(IplImage *pIplImage, UINT ID);
	void SetButtonState(void);
	
private:
	int m_nCanny_P1;
	int m_nCanny_P2;
	int m_nCanny_P3;
	
	int m_nLaplace_P1;
	
	int m_nSobel_P1;
	int m_nSobel_P2;
	int m_nSobel_P3;
	
	int m_nAntNumber;
	int m_nAntIterNumber;
	double m_fAntEvaporation;
	double m_fThresholdVal;

	CProgressCtrl m_ctrlProgress;

	CAntColony *pAntColony;
	
	CWinThread *pAntColonyThread; 
	CWinThread *pProgressThread;
	
	friend UINT ProgressThreadFunc(LPVOID lpParam);
	friend UINT AntColonyThreadFunc(LPVOID lpParam);
public:
	afx_msg void OnBnClickedPreproc();
	afx_msg void OnBnClickedThreshold();
	afx_msg void OnStnDblclickImageField();
	double m_fAlpha;
	double m_fBeta;
	int m_nPixelStepA;
	int m_nPixelStepB;
	afx_msg void OnDeltaposThresholdValSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposAntNumberSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposAntIterNumSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposPixelStepaSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposPixelStepbSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposAntAlphaSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposAntBetaSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposAntEvaporationSpin(NMHDR *pNMHDR, LRESULT *pResult);
	double m_fGamma;
	afx_msg void OnDeltaposAntGammaSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeAntDistributeType();
	afx_msg void OnBnClickedThin();
	int m_nAntMemLength;
	afx_msg void OnDeltaposAntMemLengthSpin(NMHDR *pNMHDR, LRESULT *pResult);
};

UINT ProgressThreadFunc(LPVOID lpParam); 
UINT AntColonyThreadFunc(LPVOID lpParam);
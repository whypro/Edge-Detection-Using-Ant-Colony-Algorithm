#pragma once

// CWImage ����Ŀ��
#include "cv.h"
#include "highgui.h"
#include <iostream>
#include <stack>

class CWImage
{
public:
	enum GType 
	{
		CH_R = 0,	// R ����
		CH_G,		// G ����
		CH_B,		// B ����
		MAX_VAL,	// ���ֵ
		AVR_VAL,	// ƽ��ֵ
		W_AVR_VAL,
		W_CV_VAL,
		TYPE_COUNT
	};
	enum ThinType
	{
		HILDITCH,
		PAVLIDIS,
		ROSENFELD,
		SKELETON
	};
public:
	CWImage();
	virtual ~CWImage();
	IplImage *GetCurImage(void);
	bool AddImage(const std::string &imagePath);
	bool AddImage(IplImage *pImage);
	bool DropCurImage(void);
	bool Clear(void);
	bool Reset(void);
	int Size(void) const;
	bool SaveCurImage(const std::string &imagePath);
	bool Canny(double threshold1, double threshold2, int aperture_size = 3);
	bool Sobel(int xorder, int yorder, int aperture_size = 3);
	bool Smooth(int smoothtype = 2, int param1 = 3, int param2 = 0, double param3 = 0.0);
	bool GrayEX(const GType type);
	bool Gray();
	bool Roberts();
	bool Grad();
	bool Sobel();
	bool Laplace(int aperture_size = 3);
	void Thresholding(double threshold);
	bool Thin(const ThinType type);
private:
	// ϸ���㷨
	void ThinnerHilditch(void *image, unsigned long lx, unsigned long ly);
	void ThinnerPavlidis(void *image, unsigned long lx, unsigned long ly);
	void ThinnerRosenfeld(void *image, unsigned long lx, unsigned long ly);
	void ThiningDIBSkeleton(void *lpDIBBits, unsigned long lWidth, unsigned long lHeight);
protected:
	std::stack<IplImage*> imageVec;
private:
	double *gradience;
};




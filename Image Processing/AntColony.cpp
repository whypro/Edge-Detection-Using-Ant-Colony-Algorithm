// AntColony.cpp : 实现文件
//

#include "stdafx.h"
#include "AntColony.h"

// CAntColony
CAntColony::CAntColony(
	const IplImage *pImage, 
	int numberOfAnts, 
	int numberOfIterations, 
	int memLength,
	int stA, int stB, 
	double a, double b, double c,
	double evaporationRate) : 
	antNumber(numberOfAnts), 
	maxIterationNumber(numberOfIterations), 
	antMemLength(memLength),
	stepA(stA), stepB(stB), 
	alpha(a), beta(b), gamma(c),
	evaporation(evaporationRate), 
	curIterationNumber(0)
{
	// 要保证 pImage 指向位深度为 8，通道数为 1 的图像
	pOriImage = cvCreateImage(cvGetSize(pImage), IPL_DEPTH_8U, 1);
	cvCopy(pImage, pOriImage);

	envHeight = pOriImage->height;
	envWidth = pOriImage->width;
	envWidthStep = pOriImage->widthStep;

	globalPheromone = new unsigned char[pImage->imageSize];
	globalStimulus = new unsigned char[pImage->imageSize];
	globalStimulusGrad = new double[pImage->imageSize];
	TRACE("AntNumber=%d\nIterationNumber=%d\nAntMemLength=%d\n\n", antNumber, maxIterationNumber, antMemLength);
}

CAntColony::~CAntColony()
{
	if (antVec.size() > 0)
	{
		Destroy();
	}
	cvReleaseImage(&pOriImage);
	delete []globalStimulus;
	globalStimulus = NULL;
	delete []globalPheromone;
	globalPheromone = NULL;
	delete []globalStimulusGrad;
	globalStimulusGrad = NULL;
}


// CAntColony 成员函数
bool CAntColony::ResetParameters(
	int numberOfAnts, 
	int numberOfIterations,
	int memLength,
	int stA, int stB, 
	double a, double b, double c,
	double evaporationRate)
{
	antNumber = numberOfAnts;
	maxIterationNumber = numberOfIterations;
	evaporation = evaporationRate;

	return true;
}

bool CAntColony::Destroy()
{
	for (std::vector<CAnt *>::iterator it = antVec.begin(); it != antVec.end(); it++)
	{
		delete (*it);
	}
	antVec.clear();
	return true;
}

bool CAntColony::GetGlobalStimulusImage(IplImage *pImage)
{
	memcpy(pImage->imageData, globalStimulus, pImage->imageSize);
	return true;
}

bool CAntColony::GetGlobalPheromoneImage(IplImage *pImage)
{
	memcpy(pImage->imageData, globalPheromone, pImage->imageSize);
	return true;
}

int CAntColony::GetStimulusValue(CWPoint &point)
{
	return globalStimulus[point.y * envWidthStep + point.x];
}

int CAntColony::GetStimulusGradValue(CWPoint &point)
{
	return globalStimulusGrad[point.y * envWidthStep + point.x];
}

int CAntColony::GetPheromoneValue(CWPoint &point)
{
	return globalPheromone[point.y * envWidthStep + point.x];
}

int CAntColony::GetEnvHeight()
{
	return envHeight;
}

int CAntColony::GetEnvWidth()
{
	return envWidth;
}

void CAntColony::acceptPheromone(CWPoint &point, int amount)
{
	// 增量为 amount
	int tau = globalPheromone[point.y * envWidthStep + point.x] + amount;
	if (tau > 255) 
	{
		tau = 255;
	}
	globalPheromone[point.y * envWidthStep + point.x] = (unsigned char) tau;
	
}

void CAntColony::Move()
{
	// 移动
	for (curIterationNumber = 0; curIterationNumber < maxIterationNumber; ++curIterationNumber) 
	{
		for (std::vector<CAnt *>::iterator it = antVec.begin(); it != antVec.end(); it++)
		{
			(*it)->MoveToNextCell();
		}
	}
	// 蒸发
	for (int i = 0; i < pOriImage->imageSize; i++)
	{
		int tau = (int) ((1 - evaporation) * globalPheromone[i]);
		if (tau < 0) 
		{
			tau = 0;
		}
		globalPheromone[i] = tau;
	}

}

bool CAntColony::Init(DistributeType disType)
{
	if (antVec.size() > 0)
	{
		Destroy();
	}
	// 1. 将图像转为灰度图
	// 2. 调节【蒸发系数】【蚂蚁每次施放的信息量】【阈值】

	// 3. 初始化蚂蚁数量和位置
	InitGlobalPheromone();
	InitGlobalStimulus();
	InitAntPos(disType);
	// 4. 设置迭代次数
	// 5. 计算图像的梯度	+
	// 6. 计算全局激励矩阵	+
	TRACE("%d", antMemLength);
	return true;
}

bool CAntColony::InitAntPosRand()
{
	// 随机分布
	srand(time(0));
	for (int i = 0; i < antNumber; i++)
	{
		CWPoint startPos(rand() % envWidth, rand() % envHeight);
		CAnt *ant = new CAnt(this, startPos, antMemLength);
		antVec.push_back(ant);
	}
	return true;
}

bool CAntColony::InitAntPosUniform()
{
	int antCount = 0;
	for (int y = 0; y < envHeight; y += stepB)
	{
		for (int x = 0; x < envWidth; x += stepA)
		{
			CWPoint startPos(x, y);
			CAnt *ant = new CAnt(this, startPos, antMemLength);
			antVec.push_back(ant);
			++antCount;
		}
	}
	antNumber = antCount;
	return true;
}

bool CAntColony::InitAntPosSmart()
{
	// 第一步：计算出阈值
	int gradSum = 0;
	int count = 0;
	for (int y = 0; y < envHeight; ++y)
	{
		for (int x = 0; x < envWidth; ++x)
		{
			gradSum += globalStimulus[y * envWidthStep + x];
			++count;
		}
	}
	int threshold = gradSum / count;
	// 第二步，初始化蚂蚁
	int antCount = 0;
	for (int y = 0; y < envHeight; y += stepB)
	{
		for (int x = 0; x < envWidth; x += stepA)
		{
			if (globalStimulus[y * envWidthStep + x] >= threshold)
			{
				CWPoint startPos(x, y);
				CAnt *ant = new CAnt(this, startPos, antMemLength);
				antVec.push_back(ant);
				++antCount;
			}
		}
	}
	antNumber = antCount;
	return true;
}

bool CAntColony::InitAntPos(DistributeType type)
{
	switch (type)
	{
	case RANDOM_DIS:
		InitAntPosRand();
		break;
	case UNIFORM_DIS:
		InitAntPosUniform();
		break;
	case SMART_DIS:
		InitAntPosSmart();
		break;
	}
	return true;
}

bool CAntColony::InitGlobalPheromone()
{
	for (int i = 0; i < pOriImage->imageSize; ++i)
	{
		globalPheromone[i] = 1;
	}
	return true;
}

bool CAntColony::InitGlobalStimulus()
{
	memcpy(globalStimulus, pOriImage->imageData, pOriImage->imageSize);
	return true;

/*
	unsigned char *oldImageData = (unsigned char *)pOriImage->imageData;
	int width = pOriImage->width;
	int height = pOriImage->height;
	int widthStep = pOriImage->widthStep;

	// 对非边界点应用梯度算子
	for (int y = 0; y < height - 1; y++)
	{
		for (int x = 0; x < width - 1; x++)
		{
			int pdx = oldImageData[y * widthStep + (x + 1)] - oldImageData[y * widthStep + x];
			int pdy = oldImageData[y * widthStep + x] - oldImageData[(y + 1) * widthStep + x];
			globalStimulus[y * widthStep + x] = abs(pdx) + abs(pdy);
			// divide by 0
			globalStimulusGrad[y * widthStep + x] = atan(double(pdy)/pdx);
		}
	}

	// 边界点保持不变
	for (int x = width - 1, y = 0; y < height - 1; y++)
	{
		globalStimulus[y * widthStep + x] = oldImageData[y * widthStep + x];
	}
	for (int y = height - 1, x = 0; x < width - 1; x++)
	{
		globalStimulus[y * widthStep + x] = oldImageData[y * widthStep + x];
	}

	return true;*/
}

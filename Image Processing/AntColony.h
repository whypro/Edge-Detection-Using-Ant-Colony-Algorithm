#pragma once

#include "highgui.h"
#include "cv.h"
#include "Ant.h"
#include <vector>

// CAntColony 命令目标

class CAntColony
{
public:
	enum DistributeType {
		RANDOM_DIS,
		UNIFORM_DIS,
		SMART_DIS
	};

public:
	CAntColony(
		const IplImage *pImage, 
		int numberOfAnts, 
		int numberOfIterations, 
		int memLength, 
		int stA, int stB, 
		double a, double, double,
		double evaporationRate); 
	virtual ~CAntColony();

	bool Init(DistributeType disType);
	bool ResetParameters(
		int numberOfAnts, 
		int numberOfIterations,
		int memLength, 
		int stA, int stB, 
		double a, double b, double c,
		double evaporationRate);
	bool Destroy();
	void Move();

	bool GetGlobalStimulusImage(IplImage *pImage);
	bool GetGlobalPheromoneImage(IplImage *pImage);
	
	int GetStimulusValue(CWPoint &point);
	int GetStimulusGradValue(CWPoint &point);
	int GetPheromoneValue(CWPoint &point);
	
	int GetEnvHeight();
	int GetEnvWidth();
	void acceptPheromone(CWPoint &point, int amount);	// 接收蚂蚁释放的信息素
	
	// 获取最大迭代次数
	int GetMaxIterationNumber(void) const { return maxIterationNumber; }
	// 获取当前迭代次数
	int GetCurIterationNumber(void) const { return curIterationNumber; }
	
protected:
	bool InitAntPos(DistributeType type);
	bool InitAntPosRand();
	bool InitAntPosUniform();
	bool InitAntPosSmart();
	bool InitGlobalPheromone();
	bool InitGlobalStimulus();
protected:
	// 由图像决定的参数，必须在构造函数中初始化
	int envHeight;	
	int envWidth;
	int envWidthStep;
	IplImage *pOriImage;

	// 算法相关参数
	int antNumber;
	volatile int curIterationNumber;
	volatile int maxIterationNumber;
	int antMemLength;
	double evaporation;
	int stepA;
	int stepB;
	// 在Init()中初始化
	unsigned char *globalPheromone;
	unsigned char *globalStimulus;
	std::vector<CAnt *> antVec;
public:
	double alpha;
	double beta;
	double gamma;

	double *globalStimulusGrad;
};



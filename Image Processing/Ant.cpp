// Ant.cpp : 实现文件
//

#include "stdafx.h"
#include "Ant.h"
#include "AntColony.h"
#include <map>
#include <exception>
#include <algorithm>

// CAnt
CAnt::CAnt(CAntColony *pAntColony, const CWPoint &point, const int memLength)
	: pEnvironment(pAntColony)
	, curPos(point)
	, memLen(memLength)
	, isActive(true)
{
}

CAnt::~CAnt()
{
}


// CAnt 成员函数
bool CAnt::MoveToNextCell()
{
	// 如果蚂蚁生命期结束，则不移动直接返回
	if (!isActive)
	{
		return false;
	}
	int width = pEnvironment->GetEnvWidth();
	int height = pEnvironment->GetEnvHeight();

	std::vector<CWPoint> vectorDirection;
	vectorDirection.push_back(curPos.down());
	vectorDirection.push_back(curPos.right());
	vectorDirection.push_back(curPos.left());
	vectorDirection.push_back(curPos.up());
	vectorDirection.push_back(curPos.leftup());
	vectorDirection.push_back(curPos.rightup());
	vectorDirection.push_back(curPos.leftdown());
	vectorDirection.push_back(curPos.rightdown());
	// 将四个方向插入 map 容器
	std::multimap<int, CWPoint> mapDirectionProb;
	//double dirWeight[MAX_DIRECTION_NUM] = { 1, 1/2, 1/4, 1/12, 1/20 };
	double dirWeight[MAX_DIRECTION_NUM] = { 60, 30, 15, 5, 3 };
	
	for (std::vector<CWPoint>::const_iterator cit = vectorDirection.begin(); cit != vectorDirection.end(); cit++)
	{
		CWPoint tryPos = *cit;
		int prob;
		if (tryPos > CWPoint(0, 0) && tryPos < CWPoint(width - 1, height - 1)) 
		{
			if (memPos.size() == 0)
			{
				// 初次移动
				prob = pow(pEnvironment->GetPheromoneValue(tryPos), pEnvironment->alpha) * 
					pow(pEnvironment->GetStimulusValue(tryPos), pEnvironment->beta); 
			}
			else
			{
				prob = pow(pEnvironment->GetPheromoneValue(tryPos), pEnvironment->alpha) * 
					pow(pEnvironment->GetStimulusValue(tryPos), pEnvironment->beta) * 
					pow(dirWeight[getDestDirection(tryPos)], pEnvironment->gamma);
			}
// 			int prob = pow(pEnvironment->GetPheromoneValue(tryPos), pEnvironment->alpha) * 
// 				pow(pEnvironment->GetStimulusValue(tryPos), pEnvironment->beta) * 
// 				pow(dirWeight[getDestDirection(tryPos)], pEnvironment->gamma)*
// 				(1 / pEnvironment->GetStimulusGradValue(tryPos));
			mapDirectionProb.insert(std::multimap<int, CWPoint>::value_type(prob, tryPos));
		}
	}
	
	// 删除
	for (std::multimap<int, CWPoint>::iterator it = mapDirectionProb.begin(); it != mapDirectionProb.end(); )
	{
		// 避开先前点
		if (std::find(memPos.begin(), memPos.end(), it->second) != memPos.end())
		{
			// erase() 返回下一个元素的迭代器
			mapDirectionProb.erase(it++);
		}
		else
		{
			it++;
		}
	}

	
	// 移动	
	if (mapDirectionProb.size() > 0) 
	{
		int sum = 0;
		// 如何实现从i中分别以概率为Pi取出值？
		std::multimap<int, CWPoint>::iterator it;
		for (it = mapDirectionProb.begin(); it != mapDirectionProb.end(); it++)
		{
			sum += it->first;
		}
		if (sum != 0)
		{
			int random = rand() % sum;
			int leftBound = 0, rightBound = 0;
		
			for (it = mapDirectionProb.begin(); it != mapDirectionProb.end(); it++)
			{
				rightBound += it->first;
				if (random >= leftBound && random < rightBound)
				{
					curPos = it->second;
					pEnvironment->acceptPheromone(curPos, 1);
					memPos.push_back(curPos);
					while (memPos.size() > memLen)
					{
						memPos.pop_front();
					}
					break;
				}
				leftBound += it->first;
			}
		}
		else
		{
			isActive = false;
		}
		/*
		else
		{
			// 不移动
			prevPos = curPos;
			std::multimap<int, CWPoint>::reverse_iterator rit = mapDirection.rbegin();
			int maxProb = rit->first;
			it = mapDirection.lower_bound(maxProb);
			std::advance(it, rand() % mapDirection.count(maxProb));
			curPos = it->second;
			pEnvironment->acceptPheromone(curPos);
			
		}
		*/
		//TRACE("%d, %d --> %d, %d\n", prevPos.x, prevPos.y, curPos.x, curPos.y);
	}

	return true;
}


DIRECTION CAnt::getDestDirection(CWPoint nextPos)
{
	CWPoint prevPos = *memPos.rbegin();
	CWVector curDir = curPos - prevPos;
	CWVector nextDir = nextPos - curPos;
	if (nextDir == curDir || curPos == prevPos)	// 0
	{
		return FRONT;
	}
	else if (nextDir == -curDir)	// 180
	{
		return REAR;
	}
	else if (curDir * nextDir == 1)	// 45
	{
		return BIAS_FRONT;
	}
	else if (curDir * nextDir == 0)	// 90
	{
		return BESIDE;
	}
	else if (curDir * nextDir == -1)	// 135
	{
		return BIAS_REAR;
	}
}
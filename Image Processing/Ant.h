#pragma once

#include <list>
// CWPoint 数据结构
class CWPoint
{
public:
	CWPoint(int xx = 0, int yy = 0) : x(xx), y(yy) { }
	CWPoint up() const { return CWPoint(x, y - 1); }
	CWPoint down() const { return CWPoint(x, y + 1); }
	CWPoint left() const { return CWPoint(x - 1, y); }
	CWPoint right() const { return CWPoint(x + 1, y); }
	CWPoint leftup() const { return CWPoint(x - 1, y - 1); }
	CWPoint rightup() const { return CWPoint(x + 1, y - 1); }
	CWPoint leftdown() const { return CWPoint(x - 1, y + 1); }
	CWPoint rightdown() const { return CWPoint(x + 1, y + 1); }
	bool operator ==(const CWPoint &r) const { return ((x == r.x) && (y == r.y)); }
	bool operator !=(const CWPoint &r) const { return !((x == r.x) && (y == r.y)); }
	bool operator <(const CWPoint &rhs) const { return (x < rhs.x && y < rhs.y); }
	bool operator >(const CWPoint &rhs) const { return (x > rhs.x && y > rhs.y); }
	CWPoint operator -(const CWPoint &rhs) const { return CWPoint(x - rhs.x, y - rhs.y); }
	CWPoint operator -() const { return CWPoint(-x, -y); }
	int x;
	int y;
};

class CWVector : public CWPoint
{
public:
	CWVector(const CWPoint &point) { x = point.x; y = point.y; }
	int operator *(const CWPoint &rhs) const { return x * rhs.x + y * rhs.y; }
};

class CAntColony;

// CAnt 命令目标
enum DIRECTION { FRONT, BIAS_FRONT, BESIDE, BIAS_REAR, REAR, MAX_DIRECTION_NUM };

class CAnt
{
public:
	CAnt(CAntColony *pAntColony, const CWPoint &point, const int memLength);
	virtual ~CAnt();
	bool MoveToNextCell();
protected:
	CAntColony *pEnvironment;
	CWPoint curPos;
	std::list<CWPoint> memPos;
	int memLen;
	bool isActive;

protected:
	DIRECTION getDestDirection(CWPoint nextPos);
};



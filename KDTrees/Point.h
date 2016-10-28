#pragma once
#include <iomanip>
using std::setw;

struct Point
{
	int x, y;

	Point()
	{
	}

	Point(int _x, int _y) : x(_x), y(_y)
	{
	}

	Point(const Point& p) : x(p.x), y(p.y)
	{
	}

	void set(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	bool operator==(const Point& p) const
	{
		return p.x == x && p.y == y;
	}

	friend std::ostream& operator<<(std::ostream& os, const Point p2)
	{
		return os << "(" << setw(5) << p2.x << "," << setw(5) << p2.y << ")";
	}

	unsigned int QuadraticDistance(const Point& p) const
	{
		return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y);
	}
};

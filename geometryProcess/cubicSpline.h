#pragma once
#include <vector>
struct CSpline {
	std::vector<double> x, y, a, b, c, d;
};
class CubicSpline
{
public:
	CubicSpline();
	~CubicSpline();

private:

};

#pragma once
#include <vector>
#include <Eigen/Dense>
#include <iostream>
struct CSpline {
	std::vector<double> x, y, a, b, c, d;
};
class CubicSpline
{
public:
	CSpline spline;
	CubicSpline() {};
	~CubicSpline() {};
	void addPoint(double x, double y);
	void generate(Eigen::MatrixXd* param, Eigen::VectorXd* res);
	void solver();
private:

};

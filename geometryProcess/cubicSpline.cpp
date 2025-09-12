#include "cubicSpline.h"

void CubicSpline::addPoint(double x, double y) {
	spline.x.emplace_back(x);
	spline.y.emplace_back(y);
}
void CubicSpline::generate(Eigen::MatrixXd *param, Eigen::VectorXd*res){
	int size = spline.x.size();
	if (size < 2) return;
	// �������󷽳���
	int n = size - 2;
	Eigen::MatrixXd A = Eigen::MatrixXd::Zero(n, n);
	Eigen::VectorXd B = Eigen::VectorXd::Zero(n);
	for(int i=0; i!=n; ++i){
		int index = i + 1;
		double h_index = spline.x[index + 1] - spline.x[index];
		double h_before = spline.x[index] - spline.x[index - 1];
		double v_index = 2.0 * (h_before + h_index);
		// ����B�Ĳ���
		double b_index = 6 * (spline.y[index + 1] - spline.y[index]) / h_index;
		double b_before = 6 * (spline.y[index] - spline.y[index - 1]) / h_before;

		B(i) = b_index - b_before;
		A(i, i) = v_index;
		if (i == 0) {
			A(0, 1) = h_index;
			continue;
		}
		if (i == n - 1) {
			A(n - 1, n - 2) = h_before;
			continue;
		}
		A(i, i - 1) = h_before;
		A(i, i + 1) = h_index;
	}
	*param = A;
	*res = B;
}

void CubicSpline::solver() {
	double n = spline.x.size() - 2;
	Eigen::MatrixXd paramMatrix = Eigen::MatrixXd::Zero(n, n);
	Eigen::VectorXd resMatrix = Eigen::VectorXd::Zero(n);
	generate(&paramMatrix, &resMatrix);
	// lu�ֽ�
	Eigen::PartialPivLU<Eigen::MatrixXd> lu(paramMatrix);
	Eigen::VectorXd x = lu.solve(resMatrix);
	// ת��Ϊ������ʽ��д��
	for (int i = 0; i != n; ++i) {
		// x_i�൱��m_i+1
		double h_i = spline.x[i + 1] - spline.x[i];
		if (i == 0) {
			double a = x[i] / (6 * h_i);
			double b = x[i] * spline.x[i] * 0.5 / h_i;
			double c = (3*x[i]*spline.x[i]*spline.x[i] + spline.y[i + 1] * 6 - x[i] * h_i * h_i - 6 * spline.y[i]) / (6.0 * h_i);
			spline.a.emplace_back(a);
			spline.b.emplace_back(b);
			spline.c.emplace_back(c);
			spline.d.emplace_back(spline.y[i]);
			continue;
		}
		if (i == spline.x.size() - 2) {
			double a = -x[i - 1] / (6.0 * h_i);
			double c = (spline.y[i + 1] - spline.y[i]) / h_i - h_i * x[i - 1] / 3.0;
			spline.a.emplace_back(a);
			spline.b.emplace_back(0);
			spline.c.emplace_back(c);
			spline.d.emplace_back(spline.y[i]);
			continue;
		}
		double a = (x[i] - x[i - 1]) / (6.0 * h_i);
		double b = (x[i] * spline.x[i] + x[i - 1] * spline.x[i + 1]) * 0.5 / h_i;
		double c = (3 * x[i] * spline.x[i] * spline.x[i] - 3 * x[i - 1] * spline.x[i + 1] * spline.x[i + 1] 
			+6 * spline.y[i + 1] - x[i] * h_i * h_i + x[i-1] * h_i * h_i - 6*spline.y[i])/(6 * h_i);
		spline.a.emplace_back(a);
		spline.b.emplace_back(b);
		spline.c.emplace_back(c);
		spline.d.emplace_back(spline.y[i]);
	}
}
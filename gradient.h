#ifndef GRADIENT_H
#define GRADIENT_H

struct gradient{
	double x, y;
	gradient();
};

gradient operator+(const gradient& lhs, const gradient& rhs);
gradient operator-(const gradient& lhs, const gradient& rhs);
gradient operator*(const double& lhs, const gradient& rhs);
gradient operator*(const gradient& lhs, const double& rhs);
gradient operator/(const gradient& lhs, const double& rhs);
double dotProduct(gradient A, gradient B);

#endif
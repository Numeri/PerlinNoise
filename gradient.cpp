#include "gradient.h"

gradient::gradient() : x(0.0), y(0.0)
{}

gradient operator+(const gradient& lhs, const gradient& rhs)
{
	gradient temp;
	temp.x = lhs.x + rhs.x;
	temp.y = lhs.y + rhs.y;
	return temp;
}

gradient operator-(const gradient& lhs, const gradient& rhs)
{
	gradient temp;
	temp.x = lhs.x - rhs.x;
	temp.y = lhs.y - rhs.y;
	return temp;
}

gradient operator*(const double& lhs, const gradient& rhs)
{
	gradient temp;
	temp.x = lhs * rhs.x;
	temp.y = lhs * rhs.y;
	return temp;
}

gradient operator*(const gradient& lhs, const double& rhs)
{
	gradient temp;
	temp.x = lhs.x * rhs;
	temp.y = lhs.y * rhs;
	return temp;
}

gradient operator/(const gradient& lhs, const double& rhs)
{
	gradient temp;
	temp.x = lhs.x / rhs;
	temp.y = lhs.y / rhs;
	return temp;
}

double dotProduct(gradient A, gradient B)
{
	return A.x*B.x + A.y*B.y;
}
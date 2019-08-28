#include <iostream>
#include "Runtime/Core/Math/ScltMath.h"

using namespace scarlett;

int main() {
	Matrix3f m;
	m << 1, 2, 3, 4, 5, 6, 7, 8, 9;
	std::cout << m << std::endl << std::endl;

	Matrix3f t = m.transpose();
	std::cout << t << std::endl << std::endl;

	Matrix4f r = Matrix4f::Random();
	std::cout << r << std::endl << std::endl;

	auto translation = Affine3f(Translation3f(1.0f, 2.0f, 3.0f));
	std::cout << translation.matrix() << std::endl << std::endl;

	auto scale = Affine3f(Scaling(2.0f, 2.0f, 2.0f));
	std::cout << scale.matrix() << std::endl << std::endl;

	Vector3f eye = { 1.0f, 1.0f, 1.0f };
	Vector3f lookat = { 0.0f, 0.0f, 0.0f };
	Vector3f up = { 0.0f, 1.0f, 0.0f };

	auto view1 = BuildViewLookatLH(eye, lookat, up);
	std::cout << view1 << std::endl << std::endl;
	auto view2 = BuildViewLookatRH(eye, lookat, up);
	std::cout << view2 << std::endl << std::endl;

	auto p1 = BuildOrthoRH(1024.0f, 768.0f, 1.0f, 1000.0f);
	std::cout << p1 << std::endl << std::endl;
	auto p2 = BuildOrthoLH(1024.0f, 768.0f, 1.0f, 1000.0f);
	std::cout << p2 << std::endl << std::endl;

	auto p3 = BuildPerspectiveFovRH(PI * 0.5, 1024.0f / 768.0f, 1.0f, 1000.0f);
	std::cout << p3 << std::endl << std::endl;
	auto p4 = BuildPerspectiveFovLH(PI * 0.5, 1024.0f / 768.0f, 1.0f, 1000.0f);
	std::cout << p4 << std::endl << std::endl;

	return 0;
}

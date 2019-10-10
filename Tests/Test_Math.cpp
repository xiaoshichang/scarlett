#include <iostream>
#include "Runtime/Core/Math/ScltMath.h"

using namespace std;
using namespace scarlett;

int main() {
	cout << "PI:" << PI << endl;
	cout << "TWO_PI:" << TWO_PI << endl;

	Vector3f v4 = { 1, 2, 3 };
	Vector3f v5 = { 1, 2, 3 };


	Vector3f v6 = { 1, 2, 4 };


	Vector2f v21 = { 100, 99 };
	Vector2f v22 = { -1, 0.12 };
	Vector2f v23;
	VectorAdd(v23, v21, v22);
	cout << "VectorAdd of " << v21 << " and " << v22 << " is " << v23 << endl;
	Vector2f v7 = { 99, 99.12 };


	Vector3f v31 = { 1, 2, 3 };
	Vector3f v32 = { 2, 2, 2 };
	float r1;
	r1 = DotProduct(v31, v32);
	cout << "DotProduct of " << v31 << " and " << v32 << " is " << r1 << endl;
	if (r1 != 12.0f) {
		return 1;
	}

	Vector3f v1 = { 1, 2, 3 };
	Vector3f v2 = { 2, 3, 4 };
	Vector3f v3;
	CrossProduct(v3, v1, v2);
	cout << "CrossProduct of " << v1 << " and " << v2 << " is " << v3 << endl;
	Vector3f v9 = { -1, 2, -1 };

	Vector3f v10 = { 5, 0, 0 };
	Vector3f v11 = { 1, 0, 0 };
	Normalize(v10);


	Vector3f v12 = { 0, 2, 0 };
	Vector3f v13 = { 0, 1, 0 };
	Normalize(v12);


	Vector3f v14 = { 1, 1, 1 };
	Normalize(v14);
	cout << "Normalize " << "{ 1, 1, 1 }" << " is " << v14 << endl;

	Matrix4x4f m1 = { {	{
		{ 1, 2, 3, 4 },
		{ 1, 2, 3, 4 },
		{ 1, 2, 3, 4 },
		{ 1, 2, 3, 4 },
		} } };

	Matrix4x4f m2 = { { {
		{ 1, 2, 3, 4 },
		{ 1, 2, 3, 4 },
		{ 1, 2, 3, 4 },
		{ 1, 2, 3, 4 },
		} } };

	Matrix4x4f m3, m4;
	BuildMatrixTranslation(m3, 1, 1, 1);
	BuildMatrixTranslation(m4, -1, -1, 0);

	Matrix<float, 3, 3> m5 = { { {
		{ 1, 3, 1},
		{ 2, 1, 1 },
		{ 2, 2, 1 },
		} } };

	Matrix<float, 3, 3> m7 = { { {
		{ -1, -1, 2},
		{ 0, -1, 1 },
		{ 2, 4, -5 },
		} } };

	Matrix<float, 3, 3> m6;
	MatrixInverse(m6, m5);
	cout << m6 << endl;
	MatrixMultiBy(m6, m5, m7);
	cout << m6 << endl;

	return 0;
}

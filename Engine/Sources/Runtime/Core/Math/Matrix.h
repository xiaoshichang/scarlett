#pragma once

#include <cstdint>
#include <iostream>
#include <limits>
#include <math.h>
#include <algorithm>
#include "Vector.h"


namespace scarlett {

	template<typename T, size_t RowSize, size_t ColSize>
	constexpr size_t ElementCount(T(&)[RowSize][ColSize]) { return RowSize * ColSize; }

	template <typename T, int ROWS, int COLS>
	struct Matrix {
		union {
			T data[ROWS][COLS];
		};

		T* operator[](int rowIndex) {
			return data[rowIndex];
		}

		const T* operator[](int rowIndex) const {
			return data[rowIndex];
		}

		operator T*() {
			return &data[0][0];
		}

		operator const T*() const {
			return static_cast<const T*>(&data[0][0]);
		};

		Matrix& operator=(const T* _data) {
			memcpy(data, _data, ROWS * COLS * sizeof(T));
			return *this;
		}

		bool operator==(const Matrix& m) {
			for (int r = 0; r < ROWS; ++r) {
				for (int c = 0; c < COLS; ++c) {
					if (data[r][c] != m[r][c])
						return false;
				}
			}
			return true;
		}

		template <int _ROWS, int _COLS>
		bool operator!=(const Matrix<T, _ROWS, _COLS>& m) {
			if ((_ROWS != ROWS) || (_COLS != COLS)) {
				return true;
			}
			for (int r = 0; r < ROWS; ++r) {
				for (int c = 0; c < COLS; ++c) {
					if (data[r][c] != m[r][c])
						return true;
				}
			}
			return false;
		}
	};

	typedef Matrix<float, 4, 4> Matrix4x4f;
	typedef Matrix<float, 3, 3> Matrix3x3f;

	template <typename T, int ROWS, int COLS>
	inline void Transpose(Matrix<T, COLS, ROWS>& result, const Matrix<T, ROWS, COLS>& m) {
		for (int r = 0; r < ROWS; ++r) {
			for (int c = 0; c < COLS; ++c) {
				result[c][r] = m[r][c];
			}
		}
	}

	template <typename T, int ROWS, int COLS>
	inline void TransposeInPlace(Matrix<T, ROWS, COLS>& m) {
		Matrix<T, ROWS, COLS> result;
		for (int r = 0; r < ROWS; ++r) {
			for (int c = 0; c < COLS; ++c) {
				result[c][r] = m[r][c];
			}
		}
		m = result;
	}

	template <typename T, int ROWS, int COLS>
	inline void MatrixAdd(Matrix<T, ROWS, COLS>& result, const Matrix<T, ROWS, COLS>& m1, const Matrix<T, ROWS, COLS>& m2) {
		for (int r = 0; r < ROWS; ++r) {
			for (int c = 0; c < COLS; ++c) {
				result[r][c] = m1[r][c] + m2[r][c];
			}
		}
	}

	template <typename T, int ROWS, int COLS>
	inline void MatrixSub(Matrix<T, ROWS, COLS>& result, const Matrix<T, ROWS, COLS>& m1, const Matrix<T, ROWS, COLS>& m2) {
		for (int r = 0; r < ROWS; ++r) {
			for (int c = 0; c < COLS; ++c) {
				result[r][c] = m1[r][c] - m2[r][c];
			}
		}
	}


	template <typename T, int Da, int Db, int Dc>
	inline void MatrixMultiBy(Matrix<T, Da, Dc>& result, const Matrix<T, Da, Db>& m1, const Matrix<T, Db, Dc>& m2) {
		Matrix<float, Da, Dc> tmp;
		for (int a = 0; a < Da; ++a) {
			for (int c = 0; c < Dc; ++c) {
				T sum = 0;
				for (int b = 0; b < Db; ++b) {
					sum += m1[a][b] * m2[b][c];
				}
				tmp[a][c] = sum;
			}
		}
		for (int a = 0; a < Da; ++a) {
			for (int c = 0; c < Dc; ++c) {
				result[a][c] = tmp[a][c];
			}
		}
	}

	template <typename T, int Da, int Db, int Dc>
	Matrix<float, Da, Dc> operator *(const Matrix<T, Da, Db>& m1, const Matrix<T, Db, Dc>& m2) {
		Matrix<float, Da, Dc> tmp;
		MatrixMultiBy(tmp, m1, m2);
		return tmp;
	}

	template <typename T, int d>
	inline void MatrixSwapCols(Matrix<T, d, d> m, int col1, int col2)
	{
		for (int row = 0; row <= d - 1; row++) {
			T t = m[row][col1];
			m[row][col1] = m[row][col2];
			m[row][col2] = t;
		}
	}

	template <typename T, int d>
	inline void MatrixSwapRows(Matrix<T, d, d> m, int row1, int row2)
	{
		for (int col = 0; col <= d - 1; col++) {
			T t = m[row1][col];
			m[row1][col] = m[row2][col];
			m[row2][col] = t;
		}
	}

	template <typename T, int d>
	inline float MatrixInverse(Matrix<T, d, d>& out, const Matrix<T, d, d>& in) {
		Matrix<T, d, d> m = in;
		float fDet = 1.0f;
		int f = 1;
		int is[d];
		int js[d];
		int k, i, j, l;
		int dd = d;

		for (k = 0; k < d; k++)
		{
			float fMax = 0.0f;
			for (i = k; i < d; i++)
			{
				for (j = k; j < d; j++)
				{
					const float f = abs(m[i][j]);
					if (f > fMax)
					{
						fMax = f;
						is[k] = i;
						js[k] = j;
					}
				}
			}
			if (abs(fMax) < 0.00000001f)
				return 0;
			if (is[k] != k)
			{
				f = -f;
				for (l = 0; l < d; ++l) {
					T temp = m[k][l];
					m[k][l] = m[is[k]][l];
					m[is[k]][l] = temp;
				}
			}
			if (js[k] != k)
			{
				f = -f;
				for (l = 0; l < d; ++l) {
					T temp = m[l][k];
					m[l][k] = m[l][js[k]];
					m[l][js[k]] = temp;
				}
			}
			fDet *= m[k][k];
			m[k][k] = 1.0f / m[k][k];

			for (j = 0; j < d; j++)
			{
				if (j != k)
					m[k][j] *= m[k][k];
			}

			for (i = 0; i < d; i++)
			{
				if (i != k)
				{
					for (j = 0; j < d; j++)
					{
						if (j != k)
							m[i][j] = m[i][j] - m[i][k] * m[k][j];
					}
				}
			}
			for (i = 0; i < d; i++)
			{
				if (i != k)
					m[i][k] *= -m[k][k];
			}
		}
		for (k = d - 1; k >= 0; k--)
		{
			if (js[k] != k)
			{
				for (l = 0; l < d; ++l) {
					T temp = m[k][l];
					m[k][l] = m[js[k]][l];
					m[js[k]][l] = temp;
				}
			}
			if (is[k] != k)
			{
				for (l = 0; l < d; ++l) {
					T temp = m[l][k];
					m[l][k] = m[l][is[k]];
					m[l][is[k]] = temp;
				}
			}
		}
		out = m;
		return fDet * f;
	}

	inline Vector3f TransformPoint(const Matrix4x4f& matrix, Vector3f& point)
	{
		Vector4f hSpace = Vector4f(point.x, point.y, point.z, 1);
		Vector4f tmp = Vector4f(0, 0, 0, 0);

		for (int r = 0; r < 4; ++r) {
			for (int c = 0; c < 4; ++c) {
				tmp[r] += hSpace[c] * matrix[r][c];
			}
		}
		return Vector3f(tmp.x, tmp.y, tmp.z);
	}

	inline Vector3f TransformVector(const Matrix4x4f& matrix, Vector3f& vector)
	{
		Vector4f hSpace = Vector4f(vector.x, vector.y, vector.z, 0);
		Vector4f tmp = Vector4f(0, 0, 0, 0);

		for (int r = 0; r < 4; ++r) {
			for (int c = 0; c < 4; ++c) {
				tmp[r] += hSpace[c] * matrix[r][c];
			}
		}
		return Vector3f(tmp.x, tmp.y, tmp.z);
	}

	inline Vector3f TransformVector(const Matrix3x3f& matrix, Vector3f& vector)
	{
		Vector3f tmp = Vector3f(0, 0, 0);
		for (int r = 0; r < 3; ++r) {
			for (int c = 0; c < 3; ++c) {
				tmp[r] += vector[c] * matrix[r][c];
			}
		}
		return tmp;
	}

	inline void BuildMatrixRotationX(Matrix4x4f& matrix, const float radio) {
		float c = cosf(radio), s = sinf(radio);
		Matrix4x4f tmp = { { {
			{ 1.0f,	0.0f, 0.0f, 0.0f },
			{ 0.0f,	   c,   -s, 0.0f },
			{ 0.0f,    s,    c, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
			} } };
		matrix = tmp;
	}

	inline void BuildMatrixRotationY(Matrix4x4f& matrix, const float radio) {
		float c = cosf(radio), s = sinf(radio);
		Matrix4x4f tmp = { { {
			{	 c,	0.0f,    s, 0.0f },
			{ 0.0f,	1.0f, 0.0f, 0.0f },
			{	-s, 0.0f,    c, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
			} } };
		matrix = tmp;
	}

	inline void BuildMatrixRotationZ(Matrix4x4f& matrix, const float radio) {
		float c = cosf(radio), s = sinf(radio);
		Matrix4x4f tmp = { { {
			{	 c,	  -s, 0.0f, 0.0f },
			{	 s,	   c, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
			} } };
		matrix = tmp;
	}

	inline void BuildMatrixYawPitchRoll(Matrix4x4f& matrix, const float yaw, const float pitch, const float roll) {
		float cYaw, cPitch, cRoll, sYaw, sPitch, sRoll;
		// Get the cosine and sin of the yaw, pitch, and roll.
		cYaw = cosf(yaw);
		cPitch = cosf(pitch);
		cRoll = cosf(roll);

		sYaw = sinf(yaw);
		sPitch = sinf(pitch);
		sRoll = sinf(roll);

		Matrix4x4f tmp = { { {
			{ (cRoll * cYaw) + (sRoll * sPitch * sYaw),		(-sRoll * cYaw) + (cRoll * sPitch * sYaw),	(cPitch * sYaw),		0.0f},
			{ (sRoll * cPitch), 							(cRoll * cPitch),							-sPitch,				0.0f},
			{ (cRoll * -sYaw) + (sRoll * sPitch * cYaw),	(sRoll * sYaw) + (cRoll * sPitch * cYaw),	(cPitch * cYaw),		0.0f},
			{ 0.0f,											0.0f,										0.0f,					1.0f}
			} } };
		matrix = tmp;
	}

	inline void BuildMatrixViewLookatRH(Matrix4x4f& matrix, const Vector3f& position, const Vector3f& lookAt, const Vector3f& up) {
		Vector3f xaxis, yaxis, zaxis;
		float tmp1, tmp2, tmp3;

		VectorSub(zaxis, position, lookAt);
		Normalize(zaxis);

		CrossProduct(xaxis, up, zaxis);
		Normalize(xaxis);

		CrossProduct(yaxis, zaxis, xaxis);

		tmp1 = -1 * DotProduct(xaxis, position);
		tmp2 = -1 * DotProduct(yaxis, position);
		tmp3 = -1 * DotProduct(zaxis, position);

		Matrix4x4f tmp = { {{
			{ xaxis.x, xaxis.y, xaxis.z, tmp1},
			{ yaxis.x, yaxis.y, yaxis.z, tmp2},
			{ zaxis.x, zaxis.y, zaxis.z, tmp3},
			{ 0,	   0,		0,		 1.0f}
			}} };
		matrix = tmp;
	}

	inline void BuildMatrixViewLookatLH(Matrix4x4f& matrix, const Vector3f& position, const Vector3f& lookAt, const Vector3f& up) {
		Vector3f xaxis, yaxis, zaxis;
		float tmp1, tmp2, tmp3;

		VectorSub(zaxis, lookAt, position);
		Normalize(zaxis);

		CrossProduct(xaxis, up, zaxis);
		Normalize(xaxis);

		CrossProduct(yaxis, zaxis, xaxis);

		tmp1 = -1 * DotProduct(xaxis, position);
		tmp2 = -1 * DotProduct(yaxis, position);
		tmp3 = -1 * DotProduct(zaxis, position);

		Matrix4x4f tmp = { {{
			{ xaxis.x, xaxis.y, xaxis.z, tmp1},
			{ yaxis.x, yaxis.y, yaxis.z, tmp2},
			{ zaxis.x, zaxis.y, zaxis.z, tmp3},
			{ 0,	   0,		0,		 1.0f}
			}} };
		matrix = tmp;
	}

	inline void BuildMatrixPerspectiveFovLH(Matrix4x4f& matrix, const float fov, const float aspect, const float zn, const float zf) {
		auto yscale = 1.0f / tanf(fov * 0.5f);
		auto xscale = yscale / aspect;

		Matrix4x4f tmp = { {{
		{ xscale,		0.0f,				0.0f,									  0.0f },
		{ 0.0f,			yscale,				0.0f,									  0.0f },
		{ 0.0f,			0.0f,				zf / (zf - zn),			(-zn * zf) / (zf - zn) },
		{ 0.0f,			0.0f,				1.0f,									  0.0f }
		}} };
		matrix = tmp;
	}

	inline void BuildMatrixPerspectiveFovRH(Matrix4x4f& matrix, const float fov, const float aspect, const float zn, const float zf) {
		auto yscale = 1.0f / tanf(fov * 0.5f);
		auto xscale = yscale / aspect;

		Matrix4x4f tmp = { {{
		{ xscale,		0.0f,				0.0f,									  0.0f },
		{ 0.0f,			yscale,				0.0f,									  0.0f },
		{ 0.0f,			0.0f,				zf / (zn - zf),			(zn * zf) / (zn - zf) },
		{ 0.0f,			0.0f,				-1.0f,									  0.0f }
		}} };
		matrix = tmp;
	}

	inline void BuildMatrixOrthoLH(Matrix4x4f& matrix, const float width, const float height, const float zn, const float zf) {
		Matrix4x4f tmp = { {{
		{ 2 / width,		0.0f,				0.0f,									  0.0f },
		{ 0.0f,			2 / height,			0.0f,									  0.0f },
		{ 0.0f,			0.0f,				1.0f / (zf - zn),				-zn / (zf - zn)},
		{ 0.0f,			0.0f,				0.0f,									  1.0f }
		}} };
		matrix = tmp;
	}

	inline void BuildMatrixOrthoRH(Matrix4x4f& matrix, const float width, const float height, const float zn, const float zf) {
		Matrix4x4f tmp = { {{
		{ 2 / width,	0.0f,				0.0f,									  0.0f },
		{ 0.0f,			2 / height,			0.0f,									  0.0f },
		{ 0.0f,			0.0f,				1.0f / (zn - zf),				 zn / (zn - zf)},
		{ 0.0f,			0.0f,				0.0f,									  1.0f }
		}} };
		matrix = tmp;
	}

	inline void BuildMatrixTranslation(Matrix4x4f& matrix, const float x, const float y, const float z) {
		Matrix4x4f translation = { { {
			{ 1.0f, 0.0f, 0.0f,    x },
			{ 0.0f, 1.0f, 0.0f,    y },
			{ 0.0f, 0.0f, 1.0f,    z },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
			} } };
		matrix = translation;
	}

	inline Matrix4x4f BuildMatrixTranslation(const float x, const float y, const float z) {
		Matrix4x4f translation = { { {
			{ 1.0f, 0.0f, 0.0f,    x },
			{ 0.0f, 1.0f, 0.0f,    y },
			{ 0.0f, 0.0f, 1.0f,    z },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
			} } };
		return translation;
	}

	inline void BuildMatrixScale(Matrix4x4f& matrix, const float x, const float y, const float z) {
		Matrix4x4f scale = { { {
			{    x, 0.0f, 0.0f, 0.0f },
			{ 0.0f,    y, 0.0f, 0.0f },
			{ 0.0f, 0.0f,    z, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
			} } };
		matrix = scale;
	}

	inline void BuildMatrixRotationAxis(Matrix4x4f& matrix, const Vector3f& axis, const float radio) {
		float c = cosf(radio), s = sinf(radio), one_minus_c = 1.0f - c;

		Matrix4x4f tmp = { {{
			{ c + axis.x * axis.x * one_minus_c,			axis.x * axis.y * one_minus_c - axis.z * s,		axis.x * axis.z * one_minus_c + axis.y * s,		0.0f },
			{ axis.x * axis.y * one_minus_c + axis.z * s,	c + axis.y * axis.y * one_minus_c,				axis.y * axis.z * one_minus_c - axis.x * s,		0.0f },
			{ axis.x * axis.z * one_minus_c - axis.y * s,   axis.y * axis.z * one_minus_c + axis.x * s,		c + axis.z * axis.z * one_minus_c,				0.0f },
			{ 0.0f,											0.0f,											0.0f,											1.0f }
		}} };
		matrix = tmp;
	}

	// https://krasjet.github.io/quaternion/quaternion.pdf
	// page 33
	inline void MatrixRotationQuaternion(Matrix4x4f& matrix, Quaternion q) {
		Matrix4x4f tmp = { {{
			{ 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z,	2.0f * q.x * q.y - 2.0f * q.w * q.z,		2.0f * q.x * q.z + 2.0f * q.w * q.y,						0.0f},
			{ 2.0f * q.x * q.y + 2.0f * q.w * q.z,			1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z, 2.0f * q.y * q.z - 2.0f * q.y * q.z - 2.0f * q.w * q.x,		0.0f},
			{ 2.0f * q.x * q.z - 2.0f * q.w * q.y,			2.0f * q.y * q.z + 2.0f * q.w * q.x,		1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y,					0.0f},
			{ 0.0f,											0.0f,										0.0f,														1.0f}
		}} };
		matrix = tmp;
	}

	inline Matrix4x4f MatrixRotationQuaternion(Quaternion q) {
		Matrix4x4f tmp;
		MatrixRotationQuaternion(tmp, q);
		return tmp;
	}

	inline void BuildMatrixIdentity(Matrix4x4f& matrix) {
		Matrix4x4f identity = { { {
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
			} } };
		matrix = identity;
	}

	inline Matrix4x4f BuildMatrixIdentity() {
		Matrix4x4f identity = { { {
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
			} } };
		return identity;
	}
}
#pragma once
#include <Eigen3/include/eigen3/Eigen/Dense>
#include <Eigen3/include/eigen3/Eigen/Geometry>

using namespace Eigen;

namespace scarlett {
	const float PI = 3.141592f;

	inline Matrix4f BuildViewLookatRH(const Vector3f& eye, const Vector3f& lookat, const Vector3f& up) {
		Vector3f xaxis, yaxis, zaxis;
		float tmp1, tmp2, tmp3;
		zaxis = eye - lookat;
		zaxis.normalize();

		xaxis = up.cross(zaxis);
		xaxis.normalize();

		yaxis = zaxis.cross(xaxis);

		tmp1 = -1 * xaxis.dot(eye);
		tmp2 = -1 * yaxis.dot(eye);
		tmp3 = -1 * zaxis.dot(eye);

		Matrix4f tmp;
		tmp << 
			xaxis.x(), xaxis.y(), xaxis.z(), tmp1,
			yaxis.x(), yaxis.y(), yaxis.z(), tmp2,
			zaxis.x(), zaxis.y(), zaxis.z(), tmp3,
			0, 0, 0, 1.0f;
		return tmp;
	}

	inline Matrix4f BuildViewLookatLH(const Vector3f& eye, const Vector3f& lookat, const Vector3f& up) {
		Vector3f xaxis, yaxis, zaxis;
		float tmp1, tmp2, tmp3;
		zaxis = lookat - eye;
		zaxis.normalize();

		xaxis = up.cross(zaxis);
		xaxis.normalize();

		yaxis = zaxis.cross(xaxis);

		tmp1 = -1 * xaxis.dot(eye);
		tmp2 = -1 * yaxis.dot(eye);
		tmp3 = -1 * zaxis.dot(eye);

		Matrix4f tmp;
		tmp <<
			xaxis.x(), xaxis.y(), xaxis.z(), tmp1,
			yaxis.x(), yaxis.y(), yaxis.z(), tmp2,
			zaxis.x(), zaxis.y(), zaxis.z(), tmp3,
			0, 0, 0, 1.0f;
		return tmp;
	}

	inline Matrix4f BuildPerspectiveFovRH(const float fov, const float aspect, const float zn, const float zf) {
		auto yscale = 1.0f / tanf(fov * 0.5f);
		auto xscale = yscale / aspect;

		Matrix4f tmp;
		tmp <<
			xscale, 0.0f, 0.0f, 0.0f,
			0.0f, yscale, 0.0f, 0.0f,
			0.0f, 0.0f, zf / (zn - zf), (zn * zf) / (zn - zf),
			0.0f, 0.0f, -1.0f, 0.0f;
		
		return tmp;
	}

	inline Matrix4f BuildPerspectiveFovLH(const float fov, const float aspect, const float zn, const float zf) {
		auto yscale = 1.0f / tanf(fov * 0.5f);
		auto xscale = yscale / aspect;

		Matrix4f tmp;
		tmp <<
			xscale, 0.0f, 0.0f, 0.0f,
			0.0f, yscale, 0.0f, 0.0f,
			0.0f, 0.0f, zf / (zf - zn), (-zn * zf) / (zf - zn),
			0.0f, 0.0f, 1.0f, 0.0f;
		return tmp;
	}

	inline Matrix4f BuildOrthoRH(const float width, const float height, const float zn, const float zf) {
		Matrix4f tmp;
		tmp <<
			2 / width, 0.0f, 0.0f, 0.0f,
			0.0f, 2 / height, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f / (zn - zf), zn / (zn - zf),
			0.0f, 0.0f, 0.0f, 1.0f;
		return tmp;
	}

	inline Matrix4f BuildOrthoLH(const float width, const float height, const float zn, const float zf) {
		Matrix4f tmp;
		tmp <<
			2 / width, 0.0f, 0.0f, 0.0f,
			0.0f, 2 / height, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f / (zf - zn), -zn / (zf - zn),
			0.0f, 0.0f, 0.0f, 1.0f;
		return tmp;
	}

	inline Matrix4f BuildTranslationMatrix(const Vector3f& offset) {
		Matrix4f tmp;
		tmp <<
			1.0f, 0.0f, 0.0f, offset.x(),
			0.0f, 1.0f, 0.0f, offset.y(),
			0.0f, 0.0f, 1.0f, offset.z(),
			0.0f, 0.0f, 0.0f, 1.0f;
		return tmp;
	}

	inline Matrix4f BuildScaleMatrix(const Vector3f& scale) {
		Matrix4f tmp;
		tmp <<
			scale.x(), 0.0f, 0.0f, 0.0f,
			0.0f, scale.y(), 0.0f, 0.0f,
			0.0f, 0.0f, scale.z(), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f;
		return tmp;
	}

	inline Matrix4f BuildRotationMatrix(const Vector3f& axis, const float radio) {
		Matrix4f tmp;
		float c = cosf(radio), s = sinf(radio), one_minus_c = 1.0f - c;
		tmp << c + axis.x() * axis.x() * one_minus_c,
			axis.x() * axis.y() * one_minus_c - axis.z() * s,
			axis.x() * axis.z() * one_minus_c + axis.y() * s,
			0.0f,
			axis.x() * axis.y() * one_minus_c + axis.z() * s,
			c + axis.y() * axis.y() * one_minus_c,
			axis.y() * axis.z() * one_minus_c - axis.x() * s,
			0.0f,
			axis.x() * axis.z() * one_minus_c - axis.y() * s,
			axis.y() * axis.z() * one_minus_c + axis.x() * s,
			c + axis.z() * axis.z() * one_minus_c,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			1.0f;
		return tmp;
	}

	inline Matrix4f BuildRotationMatrix(const float rx, const float ry, const float rz) {

	}


}


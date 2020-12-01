#pragma once
#include "Runtime/Core/Math/ScltMath.h"

namespace scarlett
{

	class RigidBody
	{
	public:
		RigidBody();
		RigidBody(float mass, const Matrix4x4f& inertia, float impulseCoeff, float frictionCoeff);

		bool IsStatic();
		void SetStatic(bool isStatic);

		void SetMass(float m);
		float GetMass();
		float GetInverseMass();

		void SetInertia(const Matrix4x4f& inertia);
		Matrix4x4f GetInertia();
		Matrix4x4f GetInverseInertia();

		float GetImpulseCoefficient();
		void SetImpulseCoefficient(float coeff);

		float GetFrictionCoefficient();
		void SetFrictionCoefficient(float coeff);

		Vector3f GetForce();
		void SetForce(const Vector3f& force);

		Vector3f GetVelocity();
		void SetVelocity(const Vector3f& velocity);

		Vector3f GetAngularVelocity();
		void SetAngularVelocity(const Vector3f& angularVelocity);

		bool CheckIfSleep();

	protected:


	private:
		// 这一帧施加的力
		Vector3f force;

		// 当前线性速度
		Vector3f velocity;

		// 当前线性加速度
		Vector3f acceleration;

		// 当前角速度
		Vector3f angularVel;

		// 当前位置
		Vector3f position;

		// 当前朝向
		Vector3f rotation;

		// 当前缩放
		Vector3f scale;

		// 是否处于休眠状态
		bool isSleep;

		// 是否为静态物体
		bool isStatic;

		// 质量
		float mass;

		// 质量倒数
		float inverseMass;

		// 弹性系数
		// 0 - 完全非弹性碰撞
		// 1 - 完全弹性碰撞
		float impulseCoefficient;

		// 转动惯量
		Matrix4x4f inertia;

		// 转动惯量逆矩阵
		Matrix4x4f inverseInertia;

		// 摩擦系数
		float frictionCoefficient;

	};

}

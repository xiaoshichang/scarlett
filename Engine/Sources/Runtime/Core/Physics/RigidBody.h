#pragma once
#include "Runtime/Core/Math/ScltMath.h"

namespace scarlett
{

	class RigidBodyComponent;
	class RigidBody
	{
	public:
		RigidBody();

		void SetMaster(RigidBodyComponent* master);
		RigidBodyComponent* GetMaster();

		void InitializeCoeff(float mass, const Matrix3x3f& inertia, float impulseCoeff, float frictionCoeff);

		bool IsStatic();
		void SetStatic(bool isStatic);

		void SetMass(float m);
		float GetMass();
		float GetInverseMass();

		void SetInertia(const Matrix3x3f& inertia);
		Matrix3x3f GetInertia();
		Matrix3x3f GetInverseInertia();

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
		void UpdateInverseInertiaWs();

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

		// 转动惯量 - 局部空间
		Matrix3x3f inertia;

		// 转动惯量逆矩阵 - 局部空间
		Matrix3x3f inverseInertia;

		// 转动惯量逆矩阵 - 全局空间
		Matrix3x3f inverseInertiaWs;

		// 摩擦系数
		float frictionCoefficient;

		RigidBodyComponent* master;

	};

}

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
		// ��һ֡ʩ�ӵ���
		Vector3f force;

		// ��ǰ�����ٶ�
		Vector3f velocity;

		// ��ǰ���Լ��ٶ�
		Vector3f acceleration;

		// ��ǰ���ٶ�
		Vector3f angularVel;

		// �Ƿ�������״̬
		bool isSleep;

		// �Ƿ�Ϊ��̬����
		bool isStatic;

		// ����
		float mass;

		// ��������
		float inverseMass;

		// ����ϵ��
		// 0 - ��ȫ�ǵ�����ײ
		// 1 - ��ȫ������ײ
		float impulseCoefficient;

		// ת������ - �ֲ��ռ�
		Matrix3x3f inertia;

		// ת����������� - �ֲ��ռ�
		Matrix3x3f inverseInertia;

		// ת����������� - ȫ�ֿռ�
		Matrix3x3f inverseInertiaWs;

		// Ħ��ϵ��
		float frictionCoefficient;

		RigidBodyComponent* master;

	};

}

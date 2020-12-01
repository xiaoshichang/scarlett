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
		// ��һ֡ʩ�ӵ���
		Vector3f force;

		// ��ǰ�����ٶ�
		Vector3f velocity;

		// ��ǰ���Լ��ٶ�
		Vector3f acceleration;

		// ��ǰ���ٶ�
		Vector3f angularVel;

		// ��ǰλ��
		Vector3f position;

		// ��ǰ����
		Vector3f rotation;

		// ��ǰ����
		Vector3f scale;

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

		// ת������
		Matrix4x4f inertia;

		// ת�����������
		Matrix4x4f inverseInertia;

		// Ħ��ϵ��
		float frictionCoefficient;

	};

}

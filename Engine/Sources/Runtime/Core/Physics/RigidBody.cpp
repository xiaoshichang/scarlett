#include "RigidBody.h"
#include "runtime/Core/Object/Entity.h"
#include "runtime/Core/Object/Components/TransformComponent.h"

using namespace scarlett;


scarlett::RigidBody::RigidBody()
{

	isSleep = true;
	force = Vector3f(0, 0, 0);
	velocity = Vector3f(0, 0, 0);
	acceleration = Vector3f(0, 0, 0);
	angularVel = Vector3f(0, 0, 0);

	SetStatic(false);

}

void scarlett::RigidBody::SetMaster(RigidBodyComponent * master)
{
	this->master = master;
}

RigidBodyComponent * scarlett::RigidBody::GetMaster()
{
	return this->master;
}

void scarlett::RigidBody::InitializeCoeff(float mass, const Matrix3x3f& inertia, float impulseCoeff, float frictionCoeff)
{
	SetMass(mass);
	SetInertia(inertia);
	SetImpulseCoefficient(impulseCoeff);
	SetFrictionCoefficient(frictionCoeff);
}



bool scarlett::RigidBody::IsStatic()
{
	return this->isStatic;
}

void scarlett::RigidBody::SetStatic(bool isStatic)
{
	this->isStatic = isStatic;
}

void scarlett::RigidBody::SetMass(float mass)
{
	this->mass = mass;
	this->inverseMass = 1.0f / mass;
}

float scarlett::RigidBody::GetMass()
{
	return this->mass;
}

float scarlett::RigidBody::GetInverseMass()
{
	return this->inverseMass;
}

void scarlett::RigidBody::SetInertia(const Matrix3x3f & inertia)
{
	this->inertia = inertia;
	MatrixInverse(this->inverseInertia, inertia);
}

Matrix3x3f scarlett::RigidBody::GetInertia()
{
	return this->inertia;
}

Matrix3x3f scarlett::RigidBody::GetInverseInertia()
{
	return this->inverseInertia;
}

Matrix3x3f scarlett::RigidBody::GetInverseInertiaWS()
{
	return inverseInertiaWs;
}

float scarlett::RigidBody::GetImpulseCoefficient()
{
	return this->impulseCoefficient;
}

void scarlett::RigidBody::SetImpulseCoefficient(float coeff)
{
	if (coeff > 1.0f)
	{
		coeff = 1.0f;
	}
	else if (coeff < 0.0f)
	{
		coeff = 0.0f;
	}
	this->impulseCoefficient = coeff;
}

float scarlett::RigidBody::GetFrictionCoefficient()
{
	return this->frictionCoefficient;
}

void scarlett::RigidBody::SetFrictionCoefficient(float coeff)
{
	this->frictionCoefficient = coeff;
}

Vector3f scarlett::RigidBody::GetForce()
{
	return this->force;
}

void scarlett::RigidBody::SetForce(const Vector3f & force)
{
	this->force = force;
}

Vector3f scarlett::RigidBody::GetVelocity()
{
	return this->velocity;
}

void scarlett::RigidBody::SetVelocity(const Vector3f & velocity)
{
	this->velocity = velocity;
}

Vector3f scarlett::RigidBody::GetAngularVelocity()
{
	return this->angularVel;
}

void scarlett::RigidBody::SetAngularVelocity(const Vector3f & angularVelocity)
{
	this->angularVel = angularVelocity;
}

/*
检测是否能进入休眠状态
1. 速度足够小
2. 与其他物体的所有接触的穿透距离足够小
*/
bool scarlett::RigidBody::CheckIfSleep()
{
	return false;
}

/*
计算全局坐标下的惯性张量
惯性张量的坐标变化满足：
$I_w = R * I_l * R^T$
https://www.zhihu.com/question/24846969
这里I指物体在局部坐标下的惯性张量
这里R指物体的局部到全局的旋转矩阵
另外，旋转矩阵满足正交归一特性，逆等于转置

左右求逆矩阵可以得到下面等式

*/
void scarlett::RigidBody::UpdateInverseInertiaWs()
{
	TransformComponent* transform = GetMaster()->GetMaster()->GetComponent<TransformComponent>();
	Matrix3x3f world2local = transform->GetRatationMatrixGlobal2Local33();
	Matrix3x3f world2localTranposed;
	Transpose(world2localTranposed, world2local);
	inverseInertiaWs = world2localTranposed * inverseInertia * world2local;
}

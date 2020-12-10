#include "RigidBody.h"
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

#include "Runtime/Core/Physics/Collider.h"

using namespace scarlett;

void scarlett::ColliderPlane::SetPoint(Vector3f p)
{
	this->point = p;
}

Vector3f scarlett::ColliderPlane::GetPoint()
{
	return this->point;
}

void scarlett::ColliderPlane::SetNormal(Vector3f n)
{
	this->normal = n;
}

Vector3f scarlett::ColliderPlane::GetNormal()
{
	return this->normal;
}

void scarlett::ColliderBox::SetMinPoint(Vector3f minPoint)
{
	this->minPoint = minPoint;
}

Vector3f scarlett::ColliderBox::GetMinPoint()
{
	return this->minPoint;
}

void scarlett::ColliderBox::SetMaxPoint(Vector3f maxPoint)
{
	this->maxPoint = maxPoint;
}

Vector3f scarlett::ColliderBox::GetMaxPoint()
{
	return this->maxPoint;
}

void scarlett::ColliderSphere::SetCenter(Vector3f c)
{
	this->center = c;
}

Vector3f scarlett::ColliderSphere::GetCenter()
{
	return this->center;
}

void scarlett::ColliderSphere::SetRadius(float r)
{
	this->radius = r;
}

float scarlett::ColliderSphere::GetRadius()
{
	return this->radius;
}

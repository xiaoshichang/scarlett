#pragma once
#include "Runtime/Core/Math/ScltMath.h"


namespace scarlett
{
	enum ColliderType
	{
		Plane,
		Box,
		Sphere,
	};

	class RigidBodyComponent;

	class Collider
	{
	public:
		ColliderType GetColliderType()
		{
			return ct;
		}

		RigidBodyComponent* GetComponent()
		{
			return master;
		}

	protected:
		ColliderType ct;
		RigidBodyComponent* master;
	};


	class ColliderPlane : public Collider
	{
	public:
		ColliderPlane() : 
			point(0, 0, 0),
			normal(0, 1, 0)
		{
			ct = ColliderType::Plane;
		}
	private:
		Vector3f point;
		Vector3f normal;

	public:
		void SetPoint(Vector3f p);
		Vector3f GetPoint();
		void SetNormal(Vector3f n);
		Vector3f GetNormal();
	};


	class ColliderBox : public Collider
	{
	public:
		ColliderBox() : 
			minPoint(-0.5f, -0.5f, -0.5f),
			maxPoint(0.5f, 0.5f, 0.5f)
		{
			ct = ColliderType::Box;
		}

	private:
		Vector3f minPoint;
		Vector3f maxPoint;

	public:
		void SetMinPoint(Vector3f minPoint);
		Vector3f GetMinPoint();
		void SetMaxPoint(Vector3f maxPoint);
		Vector3f GetMaxPoint();


	};

	class ColliderSphere : public Collider
	{
	public:
		ColliderSphere() :
			center(0, 0, 0),
			radius(1.0f)
		{
			ct = ColliderType::Sphere;
		}
	private:
		Vector3f center;
		float radius;

	public:
		void SetCenter(Vector3f c);
		Vector3f GetCenter();
		void SetRadius(float r);
		float GetRadius();


	};

}
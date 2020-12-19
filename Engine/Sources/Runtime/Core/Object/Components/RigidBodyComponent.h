#pragma once
#include <vector>

#include "Runtime/Core/Math/ScltMath.h"
#include "Runtime/Interface/IComponent.h"
#include "Runtime/Core/Physics/RigidBody.h"
#include "Runtime/Core/Physics/Collider.h"
#include "Runtime/Core/Physics/AABB.h"

namespace scarlett
{

	class RigidBodyComponent : public IComponent
	{
	public:
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;
		virtual void Tick() noexcept;
		RigidBodyComponent();

	public:
		RigidBody* GetRigidBody();
		AABB*	GetAABB();

		void SetContactBeta(float b);
		void SetRestitution(float r);
		float GetContactBeta();
		float GetRestitution();

	private:
		RigidBody rigidBody;
		AABB aabb;

		// 描述碰撞发生后的修正参数
		float contactBeta;
		// 描述碰撞发生后的反弹效果，理解为弹性碰撞系数
		float restitution;

	};
}


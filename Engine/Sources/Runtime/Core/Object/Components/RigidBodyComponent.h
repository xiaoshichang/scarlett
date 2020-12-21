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

		void SetFriction(float f);
		void SetContactBeta(float b);
		void SetRestitution(float r);
		float GetFriction();
		float GetContactBeta();
		float GetRestitution();

	private:
		RigidBody rigidBody;
		AABB aabb;

		// 摩擦系数
		float friction;

		// 描述碰撞发生后的修正参数
		// 接近0 - 修复缓慢，但是稳定。
		// 接近1 - 快速修复，但是会出现不稳定现象。
		float contactBeta;

		// 描述碰撞发生后的反弹效果，理解为弹性碰撞系数。
		// 接近0 - 完全非弹性碰撞
		// 接近1 - 完全弹性碰撞
		float restitution;

	};
}


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

	private:
		RigidBody rigidBody;
		AABB aabb;
		
	};
}


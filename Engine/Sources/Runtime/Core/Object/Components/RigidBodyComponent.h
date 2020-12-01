#pragma once
#include <vector>

#include "Runtime/Core/Math/ScltMath.h"
#include "Runtime/Interface/IComponent.h"
#include "Runtime/Core/Physics/RigidBody.h"
#include "Runtime/Core/Physics/Collider.h"

namespace scarlett
{

	class RigidBodyComponent : public IComponent
	{
	public:
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;
		virtual void Tick() noexcept;
		RigidBodyComponent();
		RigidBodyComponent(float mass, const Matrix4x4f& inertia, float impulseCoeff, float frictionCoeff);

	public:
		RigidBody* GetRigidBody();

	private:
		RigidBody rigidBody;
		
	};
}


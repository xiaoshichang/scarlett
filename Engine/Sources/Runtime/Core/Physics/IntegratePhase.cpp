#include "IntegratePhase.h"
#include "Runtime/Core/Physics/RigidBody.h"
#include "Runtime/Core/Object/Entity.h"
#include "Runtime/Core/Object/Components/RigidBodyComponent.h"


void scarlett::IntegratePhase::integrate(std::vector<RigidBody*> activedRigidBodies, float dt)
{
	for each (RigidBody* rigidBody in activedRigidBodies)
	{
		if (rigidBody->IsStatic())
		{
			rigidBody->SetVelocity(Vector3f(0, 0, 0));
			rigidBody->SetAngularVelocity(Vector3f(0, 0, 0));
			continue;
		}

		// 更新位置
		TransformComponent* transform = rigidBody->GetMaster()->GetMaster()->GetComponent<TransformComponent>();
		auto position = transform->GetPosition();
		Vector3f newPosition;
		Vector3f v = rigidBody->GetVelocity();
		VectorAdd(newPosition, position, VectorScale(v, dt));
		transform->SetPosition(newPosition);

		// 更新朝向
		// omega: angular velocity (direction is axis, magnitude is angle)
		// https://fgiesen.wordpress.com/2012/08/24/quaternion-differentiation/
		// https://www.ashwinnarayan.com/post/how-to-integrate-quaternions/
		// https://gafferongames.com/post/physics_in_3d/

		Quaternion rotation = transform->GetRotation();

		Vector3f omega = rigidBody->GetAngularVelocity();
		dt *= 0.5f;
		Quaternion delta(omega.x * dt, omega.y * dt, omega.z * dt, 1.0f);
		Quaternion target = delta * rotation;
		transform->SetRotation(target);
		
	}
}


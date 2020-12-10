#include "IntegratePhase.h"
#include "Runtime/Core/Physics/RigidBody.h"
#include "Runtime/Core/Object/Entity.h"
#include "Runtime/Core/Object/Components/RigidBodyComponent.h"


void scarlett::IntegratePhase::integrate(std::vector<RigidBody*> activedRigidBodies, float dt)
{
	for each (RigidBody* rigidBody in activedRigidBodies)
	{
		TransformComponent* transform = rigidBody->GetMaster()->GetMaster()->GetComponent<TransformComponent>();
		auto position = transform->GetPosition();
		Vector3f newPosition;
		Vector3f v = rigidBody->GetVelocity();
		VectorAdd(newPosition, position, VectorScale(v, dt));
		transform->SetPosition(newPosition);
		
	}
}


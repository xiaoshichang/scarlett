#include "LightComponent.h"
#include "Runtime/Core/Object/Entity.h"
#include "Runtime/Core/Object/World.h"


scarlett::LightComponent::LightComponent()
{
	mType = LightType::Directional;
	mParam.mDirectional.direction = Vector4f(0, 1, 1, 0);
	mParam.mDirectional.color = Vector4f(20, 20, 20, 0);
}

int scarlett::LightComponent::Initialize() noexcept
{
	auto master = GetMaster();
	if (master) {
		auto world = master->GetWorld();
		if (world) {
			world->GetLightSystem()->AddComponent(this);
		}
	}
	return 0;
}

void scarlett::LightComponent::Finalize() noexcept
{
	auto master = GetMaster();
	if (master) {
		auto world = master->GetWorld();
		if (world) {
			world->GetLightSystem()->DeleteComponent(this);
		}
	}
}

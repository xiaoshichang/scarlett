#pragma once
#include <memory>
#include <set>
#include "Runtime/Core/Object/Entity.h"
#include "Runtime/Interface/ISystem.h"
#include "Runtime/Core/Object/Components/LightComponent.h"

namespace scarlett {
	
	class World;

	class LightSystem : public ITickableSystem {
	public:
		LightSystem(World* world);
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;
		virtual void Tick() noexcept;

	public:
		virtual void AddComponent(LightComponent* comp);
		virtual void DeleteComponent(LightComponent* comp);

	public:
		LightComponent*	GetMainDirectionalLight();

	private:
		World* mWorld;
		std::set<LightComponent*> mComponents;


	};

}
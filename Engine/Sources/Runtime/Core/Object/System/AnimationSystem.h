#pragma once
#include <memory>
#include <set>
#include "Runtime/Core/Object/Entity.h"
#include "Runtime/Interface/ISystem.h"
#include "Runtime/Core/Object/Components/SkeletonComponent.h"

#define MAX_BONE_NUM 32

namespace scarlett {

	class AnimationSystem : public ITickableSystem {
	public:
		AnimationSystem(World* world);
		virtual ~AnimationSystem();
		virtual		int		Initialize() noexcept;
		virtual		void	Finalize() noexcept;
		virtual		void	Tick() noexcept;

	public:
		virtual void AddComponent(SkeletonComponent* comp);
		virtual void DeleteComponent(SkeletonComponent* comp);


	private:
		World* mWorld;
		std::set<SkeletonComponent*> mComponents;
	};

}

#pragma once
#include <memory>
#include "Runtime/Core/Object/Entity.h"
#include "Runtime/Interface/ISystem.h"

namespace scarlett {
	class World;

	class CameraSystem : public IModule {
	public:
		CameraSystem(World* world);
		virtual		int		Initialize() noexcept;
		virtual		void	Finalize() noexcept;
	public:
		std::shared_ptr<Entity>		GetMainCamera();
		void	SetMainCamera(std::shared_ptr<Entity> camera);

	private:
		World* mWorld;

		std::shared_ptr<Entity>		mMainCamera;
	};

}
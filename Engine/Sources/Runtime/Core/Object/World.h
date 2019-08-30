#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "Runtime/Interface/IModule.h"
#include "Runtime/Core/Object/Entity.h"

#include "Runtime/Core/Object/System/MeshRenderSystem.h"
#include "Guid.hpp"

namespace scarlett {
	class Entity;
	class World : public ITickableModule {

	public:
		virtual int		Initialize() noexcept;
		virtual void	Finalize() noexcept;
		virtual void	Tick() noexcept;
		virtual void	Render() noexcept;
		
	public:
		World();

		std::shared_ptr<Entity>	CreateEntity();
		std::shared_ptr<Entity>	CreateEntity(const Guid& guid);
		void	DeleteEntity(const Guid & guid);
		std::shared_ptr<Entity>	GetEntity(const Guid& guid);
		size_t GetEntityCount();

		void LoadScene(const std::string& scenePath);
		void DumpEntities();


	public:
		MeshRenderSystem* GetMeshRenderSystem() { return mMeshRenderSystem; }

	private:
		std::unordered_map<Guid, std::shared_ptr<Entity>>	mEntities;

		// systems
		MeshRenderSystem* mMeshRenderSystem;

	};

}
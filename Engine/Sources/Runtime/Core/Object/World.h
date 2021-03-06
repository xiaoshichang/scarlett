#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "Runtime/Interface/IModule.h"

#include "Runtime/Core/Object/Entity.h"
#include "Runtime/Core/Object/System/MeshRenderSystem.h"
#include "Runtime/Core/Object/System/CameraSystem.h"
#include "Runtime/Core/Object/System/RenderDebugSystem.h"
#include "Runtime/Core/Object/System/AnimationSystem.h"
#include "Runtime/Core/Object/System/TerrainSystem.h"
#include "Runtime/Core/Object/System/LightSystem.h"
#include "Runtime/Core/Object/System/PhysicsSystem.h"

#include <boost/functional/hash.hpp>

namespace scarlett {
	class Entity;
	class Application;

	class World : public ITickableModule {

	public:
		virtual int		Initialize() noexcept;
		virtual void	Finalize() noexcept;
		virtual void	Tick() noexcept;
		virtual void	Render() noexcept;
		
	public:
		World();

		std::shared_ptr<Entity>	CreateEntity();
		std::shared_ptr<Entity>	CreateEntity(const boost::uuids::uuid& guid);
		void	DeleteEntity(const boost::uuids::uuid & guid);
		std::shared_ptr<Entity>	GetEntity(const boost::uuids::uuid& guid);
		size_t GetEntityCount();

		void LoadScene(const std::string& scenePath);
		void DumpEntities();


	public:
		MeshRenderSystem*	GetMeshRenderSystem() { return mMeshRenderSystem; }
		CameraSystem*		GetCameraSystem() { return mCameraSystem; }
		RenderDebugSystem*	GetRenderDebugSystem() { return mRenderDebugSystem; }
		AnimationSystem*	GetAnimationSystem() { return mAnimationSystem; }
		TerrainSystem*		GetTerrainSystem() { return mTerrainSystem; }
		LightSystem*		GetLightSystem() { return mLightSystem; }
		PhysicsSystem*		GetPhysicsSystem() { return mPhysicsSystem; }

	public:
		Application* mApp;
		Assimp::Importer importer2;
	private:

		std::unordered_map<boost::uuids::uuid, std::shared_ptr<Entity>, boost::hash<boost::uuids::uuid>>	mEntities;

		// systems
		MeshRenderSystem*	mMeshRenderSystem;
		CameraSystem*		mCameraSystem;
		RenderDebugSystem*	mRenderDebugSystem;
		AnimationSystem*	mAnimationSystem;
		TerrainSystem*		mTerrainSystem;
		LightSystem*		mLightSystem;
		PhysicsSystem*		mPhysicsSystem;
	};

}
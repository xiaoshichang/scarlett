#pragma once
#include <vector>

#include "Runtime/Interface/IModule.h"
#include "Runtime/Core/Memory/MemoryManager.h"

#include "Runtime/Core/Object/Components/TransformComponent.h"
#include "Runtime/Core/Object/Components/MeshRenderComponent.h"
#include "Runtime/Core/Object/Components/CameraComponent.h"
#include "Runtime/Core/Object/Components/SkeletonComponent.h"
#include "Runtime/Core/Object/Components/TerrainComponent.h"
#include "Runtime/Core/Object/Components/LightComponent.h"
#include "Runtime/Core/Object/Components/RigidBodyComponent.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


namespace scarlett {
	class World;
	class Entity : public IModule {
	public:
		virtual int		Initialize() noexcept;
		virtual int		Initialize(World* world) noexcept;
		virtual void	Finalize() noexcept;
		
	public:
		Entity();
		Entity(const boost::uuids::uuid& guid);
		virtual ~Entity();
		boost::uuids::uuid	GetGuid() const noexcept;
		void	SetGuid(const boost::uuids::uuid& guid) noexcept;

		void	AddChild(std::shared_ptr<Entity> child);
		void	RemoveChild(std::shared_ptr<Entity> child);
		Entity* GetParent();
		void	SetParent(Entity* parent);
		bool	IsChild(std::shared_ptr<Entity> child);
		size_t	GetChildrenCount();

		World*	GetWorld() { return mWorld; }

		template<typename T>T*	AddComponent();
		template<typename T>T*	GetComponent();
		template<typename T>void	RemoveComponent();
		

	protected:
		boost::uuids::uuid		mGuid;
		Entity*		mParent;
		World*		mWorld;
		std::vector<std::shared_ptr <Entity>>	mChildren;

	protected:
		TransformComponent*		mTransform;
		MeshRenderComponent*	mMeshRender;
		CameraComponent*		mCamera;
		SkeletonComponent*		mSkeleton;
		TerrainComponent*		mTerrain;
		LightComponent*			mLight;
		RigidBodyComponent*		mRigidBody;
		

	};

	template<typename T>
	T*	scarlett::Entity::AddComponent() {
		auto MemoryManager = GMemoryManager::GetInstance();
		void* comp = nullptr;
		if (std::is_same<T, TransformComponent>::value) {
			mTransform = new TransformComponent();
			mTransform->SetMaster(this);
			mTransform->Initialize();
			comp = mTransform;
		}

		else if (std::is_same<T, MeshRenderComponent>::value) {
			void* addr = MemoryManager->Allocate(sizeof(TransformComponent));
			mMeshRender = new(addr) MeshRenderComponent();
			mMeshRender->SetMaster(this);
			mMeshRender->Initialize();
			comp = mMeshRender;
		}

		else if (std::is_same<T, CameraComponent>::value) {
			mCamera = new CameraComponent();
			mCamera->SetMaster(this);
			mCamera->Initialize();
			comp = mCamera;
		}

		else if (std::is_same<T, SkeletonComponent>::value) {
			mSkeleton = new SkeletonComponent();
			mSkeleton->SetMaster(this);
			mSkeleton->Initialize();
			comp = mSkeleton;
		}

		else if (std::is_same<T, TerrainComponent>::value) {
			mTerrain = new TerrainComponent();
			mTerrain->SetMaster(this);
			mTerrain->Initialize();
			comp = mTerrain;
		}

		else if (std::is_same<T, LightComponent>::value) {
			mLight = new LightComponent();
			mLight->SetMaster(this);
			mLight->Initialize();
			comp = mLight;
		}

		else if (std::is_same<T, RigidBodyComponent>::value)
		{
			mRigidBody = new RigidBodyComponent();
			mRigidBody->SetMaster(this);
			mRigidBody->Initialize();
			comp = mRigidBody;
		}

		return (T*)comp;
	}

	template<typename T>
	T*		scarlett::Entity::GetComponent() {
		void* ret = nullptr;
		if (std::is_same<T, TransformComponent>::value) {
			ret = mTransform;
		}
		else if (std::is_same<T, MeshRenderComponent>::value) {
			ret = mMeshRender;
		}
		else if (std::is_same<T, CameraComponent>::value) {
			ret = mCamera;
		}
		else if (std::is_same<T, SkeletonComponent>::value) {
			ret = mSkeleton;
		}
		else if (std::is_same<T, TerrainComponent>::value) {
			ret = mTerrain;
		}
		else if (std::is_same<T, LightComponent>::value) {
			ret = mLight;
		}
		else if (std::is_same<T, RigidBodyComponent>::value) {
			ret = mRigidBody;
		}

		return (T*)ret;
	}

	template<typename T>
	void	scarlett::Entity::RemoveComponent() {
		auto MemoryManager = GMemoryManager::GetInstance();
		if (std::is_same<T, TransformComponent>::value) {
			mTransform->Finalize();
			delete mTransform;
			mTransform = nullptr;
		}
		else if (std::is_same<T, MeshRenderComponent>::value) {
			mMeshRender->Finalize();
			MemoryManager->Free(mMeshRender, sizeof(MeshRenderComponent));
			mMeshRender = nullptr;
		}
		else if (std::is_same<T, CameraComponent>::value) {
			mCamera->Finalize();
			delete mCamera;
			mCamera = nullptr;
		}
		else if (std::is_same<T, SkeletonComponent>::value) {
			mSkeleton->Finalize();
			delete mSkeleton;
			mSkeleton = nullptr;
		}
		else if (std::is_same<T, TerrainComponent>::value) {
			mTerrain->Finalize();
			delete mTerrain;
			mTerrain = nullptr;
		}
		else if (std::is_same<T, LightComponent>::value) {
			mLight->Finalize();
			delete mLight;
			mLight = nullptr;
		}
		else if (std::is_same<T, RigidBodyComponent>::value)
		{
			mRigidBody->Finalize();
			delete mRigidBody;
			mRigidBody = nullptr;
		}


	}

}

#pragma once
#include <vector>

#include "Runtime/Interface/IModule.h"
#include "Runtime/Core/Object/Components/TransformComponent.h"
#include "Runtime/Core/Object/Components/MeshRenderComponent.h"
#include "Runtime/Core/Object/Components/CameraComponent.h"
#include "Runtime/Core/Object/Components/SkeletonComponent.h"
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

	};

	template<typename T>
	T*	scarlett::Entity::AddComponent() {
		void* comp = nullptr;
		if (std::is_same<T, TransformComponent>::value) {
			mTransform = new TransformComponent();
			mTransform->SetMaster(this);
			mTransform->Initialize();
			comp = mTransform;
		}

		else if (std::is_same<T, MeshRenderComponent>::value) {
			mMeshRender = new MeshRenderComponent();
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
		return (T*)ret;
	}

	template<typename T>
	void	scarlett::Entity::RemoveComponent() {
		if (std::is_same<T, TransformComponent>::value) {
			mTransform->Finalize();
			delete mTransform;
			mTransform = nullptr;
		}
		else if (std::is_same<T, MeshRenderComponent>::value) {
			mMeshRender->Finalize();
			delete mMeshRender;
			mMeshRender = nullptr;
		}
		else if (std::is_same<T, CameraComponent>::value) {
			mCamera->Finalize();
			delete mCamera;
			mCamera = nullptr;
		}
		else if (std::is_same<T, CameraComponent>::value) {
			mSkeleton->Finalize();
			delete mSkeleton;
			mSkeleton = nullptr;
		}
	}

}

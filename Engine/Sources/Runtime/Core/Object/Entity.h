#pragma once
#include <vector>

#include "Guid.hpp"
#include "Runtime/Interface/IModule.h"
#include "Runtime/Core/Object/Components/TransformComponent.h"
#include "Runtime/Core/Object/Components/MeshRenderComponent.h"
#include "Runtime/Core/Object/Components/CameraComponent.h"

using namespace xg;

namespace scarlett {
	class World;
	class Entity : public IModule {
	public:
		virtual int		Initialize() noexcept;
		virtual int		Initialize(World* world) noexcept;
		virtual void	Finalize() noexcept;
		
	public:
		Entity();
		Entity(const Guid& guid);
		virtual ~Entity();
		Guid	GetGuid() const noexcept;
		void	SetGuid(const Guid& guid) noexcept;

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
		Guid		mGuid;
		Entity*		mParent;
		World*		mWorld;
		std::vector<std::shared_ptr <Entity>>	mChildren;

	protected:
		TransformComponent*		mTransform;
		MeshRenderComponent*	mMeshRender;
		CameraComponent*		mCamera;

	};

	template<typename T>
	T*	scarlett::Entity::AddComponent() {
		void* comp;
		if (std::is_same<T, TransformComponent>::value) {
			mTransform = new TransformComponent();
			mTransform->SetMaster(this);
			mTransform->Initialize();
			comp = mTransform;
		}

		if (std::is_same<T, MeshRenderComponent>::value) {
			mMeshRender = new MeshRenderComponent();
			mMeshRender->SetMaster(this);
			mMeshRender->Initialize();
			comp = mMeshRender;
		}

		if (std::is_same<T, CameraComponent>::value) {
			mCamera = new CameraComponent();
			mCamera->SetMaster(this);
			mCamera->Initialize();
			comp = mCamera;
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
		return (T*)ret;
	}

	template<typename T>
	void	scarlett::Entity::RemoveComponent() {
		if (std::is_same<T, TransformComponent>::value) {
			delete mTransform;
			mTransform = nullptr;
		}
		if (std::is_same<T, MeshRenderComponent>::value) {
			delete mMeshRender;
			mMeshRender = nullptr;
		}
		if (std::is_same<T, CameraComponent>::value) {
			delete mCamera;
			mCamera = nullptr;
		}
	}

}

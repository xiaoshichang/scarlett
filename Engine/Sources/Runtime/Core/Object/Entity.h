#pragma once
#include <vector>

#include "Guid.hpp"
#include "Runtime/Interface/IModule.h"
#include "Runtime/Core/Object/Components/TransformComponent.h"
#include "Runtime/Core/Object/Components/MeshRenderComponent.h"

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

		template<typename T>void	AddComponent();
		template<typename T>T*	GetComponent();
		template<typename T>void	RemoveComponent();
		

	protected:
		Guid		mGuid;
		Entity*		mParent;
		World*		mWorld;
		std::vector<std::shared_ptr <Entity>>	mChildren;

	protected:
		TransformComponent* mTransform;
		MeshRenderComponent* mMeshRender;

	};

	template<typename T>
	void	scarlett::Entity::AddComponent() {
		if (std::is_same<T, TransformComponent>::value) {
			mTransform = new TransformComponent();
			mTransform->SetMaster(this);
			mTransform->Initialize();
		}

		if (std::is_same<T, MeshRenderComponent>::value) {
			mMeshRender = new MeshRenderComponent();
			mMeshRender->SetMaster(this);
			mMeshRender->Initialize();
		}
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
	}

}

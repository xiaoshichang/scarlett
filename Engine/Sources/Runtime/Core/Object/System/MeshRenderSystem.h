#pragma once
#include <set>
#include "Guid.hpp"
#include "Runtime/Interface/ISystem.h"
#include "Runtime/Core/Object/Components/MeshRenderComponent.h"

using namespace xg;
namespace scarlett {

	class MeshRenderSystem : public ITickableSystem {

	public:
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;
		virtual void Tick() noexcept;

	public:
		virtual void AddComponent(MeshRenderComponent* comp);
		virtual void DeleteComponent(MeshRenderComponent* comp);
		virtual void Render();

	private:
		std::set< MeshRenderComponent*> mComponents;

	};
}
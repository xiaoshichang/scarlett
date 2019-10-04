#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Runtime/Interface/IComponent.h"
#include "Runtime/RHI/Mesh.h"

namespace scarlett {

	class MeshRenderComponent : public IComponent {
	public:
		MeshRenderComponent() : mVisible(true){}

		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;
	public:
		void SetVisible(bool v) { mVisible = v; }
		bool IsVisible() { return mVisible; }

	public:
		std::vector<std::shared_ptr<IMesh>> mMeshes;;
		bool mVisible;

	};

}
#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Runtime/Interface/IComponent.h"
#include "Runtime/RHI/RenderObject.h"

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
		std::vector<int> mMeshIdxes;
		bool mVisible;

	};

}
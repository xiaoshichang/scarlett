#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Runtime/Interface/IComponent.h"
#include "Runtime/RHI/RenderObject.h"

namespace scarlett {

	class MeshRenderComponent : public IComponent {
	public:
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;
		virtual void Render();
	public:
		void SetVisible(bool v) { mVisible = v; }
		bool IsVisible() { return mVisible; }
		std::shared_ptr<RenderObject> AddRenderObject();
		std::shared_ptr<RenderObject> GetRenderObject(int idx);
		size_t GetRenderObjectCount();
	
	private:
		bool mVisible;
		std::vector<std::shared_ptr<RenderObject>> mRenderObjects;

	};

}
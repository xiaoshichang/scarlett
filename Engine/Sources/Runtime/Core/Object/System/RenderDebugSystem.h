#pragma once
#include <unordered_map>
#include "Runtime/Interface/ISystem.h"
#include "Runtime/RHI/RenderMesh.h"

namespace scarlett {
	class World;

	class RenderDebugSystem : public IModule{
	public:
		RenderDebugSystem(World* world);
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;

		void Render() noexcept;
		
		void CreateDebugMesh() noexcept;
		void DeleteDebugMesh() noexcept;

		void CreateDebugAxis() noexcept;
		void DeleteDebugAxis() noexcept;

	private:
		World* mWorld;
		std::unordered_map<std::string, shared_ptr<RenderMesh>>		mMeshes;
	};

}
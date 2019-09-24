#pragma once
#include <unordered_map>
#include <memory>
#include "Runtime/Interface/ISystem.h"
#include "Runtime/RHI/Mesh.h"

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
		std::unordered_map<std::string, std::shared_ptr<IMesh>>		mMeshes;
	};

}
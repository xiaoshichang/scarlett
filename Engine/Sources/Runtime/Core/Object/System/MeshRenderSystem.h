#pragma once
#include <set>

#include "Runtime/Interface/ISystem.h"

#include "Runtime/RHI/GraphicsMgr.h"
#include "Runtime/RHI/Mesh.h"

#include "Runtime/Core/Object/Components/MeshRenderComponent.h"

using namespace std;

namespace scarlett {
	class World;

	class MeshRenderSystem : public ITickableSystem {

	public:
		MeshRenderSystem(World* world);

		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;
		virtual void Tick() noexcept;

	public:
		virtual void AddComponent(MeshRenderComponent* comp);
		virtual void DeleteComponent(MeshRenderComponent* comp);
		virtual void Render();

	public:
		virtual void LoadMesh(aiMesh* mesh, const aiScene* world);


	public:
		std::vector<shared_ptr<IMesh>>	mMeshes;
	private:
		World* mWorld;
		GraphicsManager* mGraphicsManager;
		std::set< MeshRenderComponent*> mComponents;

		

	};
}
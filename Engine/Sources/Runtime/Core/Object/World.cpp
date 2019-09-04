#include "Foundation/Assert.h"

#include "Runtime/Core/Object/World.h"
#include "Runtime/Core/Object/Components/MeshRenderComponent.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include <iostream>

using namespace scarlett;
using namespace std;

scarlett::World::World(Application* master) :
	mApp(master),
	mMeshRenderSystem(nullptr)
{
}

int scarlett::World::Initialize() noexcept
{
	mMeshRenderSystem = new MeshRenderSystem(this);
	mMeshRenderSystem->Initialize();

	return 0;
}

void scarlett::World::Finalize() noexcept
{
	mEntities.clear();

	mMeshRenderSystem->Finalize();
}

void scarlett::World::Tick() noexcept
{
	mMeshRenderSystem->Tick();
}

void scarlett::World::Render() noexcept
{
	mMeshRenderSystem->Render();
}

std::shared_ptr<Entity> scarlett::World::CreateEntity()
{
	auto entity = std::make_shared<Entity>();
	entity->Initialize(this);
	mEntities[entity->GetGuid()] = entity;
	return entity;
}

std::shared_ptr<Entity> scarlett::World::CreateEntity(const Guid & guid)
{
	if (mEntities[guid]) {
		return nullptr;
	}

	auto entity = std::make_shared<Entity>(guid);
	entity->Initialize(this);
	mEntities[guid] = entity;
	return entity;
}

void scarlett::World::DeleteEntity(const Guid & guid)
{
	auto entity = mEntities[guid];
	if (entity) {
		entity->Finalize();
		mEntities.erase(guid);
	}
}

std::shared_ptr<Entity> scarlett::World::GetEntity(const Guid & guid)
{
	if (!mEntities[guid]) {
		return nullptr;
	}
	return mEntities[guid];
}

size_t scarlett::World::GetEntityCount() {
	return mEntities.size();
}

void scarlett::World::LoadScene(const std::string& scenePath) {
	Assimp::Importer importer2;
	const aiScene* scene = importer2.ReadFile(scenePath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);
	SCARLETT_ASSERT(scene);

	for (unsigned int j = 0; j < scene->mNumMeshes; ++j) {
		auto mesh = scene->mMeshes[j];
		mMeshRenderSystem->LoadMesh(mesh);
	}
	
	for (unsigned int i = 0; i < scene->mRootNode->mNumChildren; ++i) {
		auto child = scene->mRootNode->mChildren[i];
		if (child->mNumMeshes <= 0) {
			continue;
		}

		auto entity = CreateEntity();
		aiVector3D scaling, rotation, position;
		child->mTransformation.Decompose(scaling, rotation, position);

		auto transformation = entity->GetComponent<TransformComponent>();
		transformation->SetPosition(Vector3f(position.x, position.y, position.z));
		transformation->SetRotation(Vector3f(rotation.x, rotation.y, rotation.z));
		transformation->SetScale(Vector3f(scaling.x, scaling.y, scaling.z));

		auto comp = entity->AddComponent<MeshRenderComponent>();
		
		for (unsigned int j = 0; j < child->mNumMeshes; ++j) {
			auto midx = child->mMeshes[j];
			comp->mMeshIdx.push_back(midx);
		}
	}
}

void scarlett::World::DumpEntities()
{
	cout << "dump entities:" << endl;
	for (auto pair : mEntities) {
		auto guid = pair.first;
		auto entity = pair.second;
		
		cout << "guid: " << guid << endl;
		cout << "transform component:" << endl;
		auto position = entity->GetComponent<TransformComponent>()->GetPosition();
		cout << "position: " <<  "(" << position.x() << "," << position.y() << "," << position.z() << ")" << endl;
		
		auto meshRender = entity->GetComponent<MeshRenderComponent>();
		if (meshRender) {
			cout << "MeshRenderComponent:" <<  endl;
			cout << "MeshIndex:";
			for (int i = 0; i < meshRender->mMeshIdx.size(); ++i) {
				cout << meshRender->mMeshIdx[i] << " ";
			}
			cout << "Mesh name:";
			for (int i = 0; i < meshRender->mMeshIdx.size(); ++i) {
				auto idx = meshRender->mMeshIdx[i];
				auto mesh = mMeshRenderSystem->mMeshes[idx];
			}
			cout << endl;
		}
		cout << endl;
	}
}

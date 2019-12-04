#include "Foundation/Assert.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/Core/Object/World.h"
#include "Runtime/Core/Object/Components/MeshRenderComponent.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include <iostream>

using namespace scarlett;
using namespace std;

scarlett::World::World() :
	mMeshRenderSystem(nullptr)
{
}

int scarlett::World::Initialize() noexcept
{
	mTerrainSystem = new TerrainSystem(this);
	mTerrainSystem->Initialize();

	mMeshRenderSystem = new MeshRenderSystem(this);
	mMeshRenderSystem->Initialize();

	mAnimationSystem = new AnimationSystem(this);
	mAnimationSystem->Initialize();
	
	mCameraSystem = new CameraSystem(this);
	mCameraSystem->Initialize();

	mRenderDebugSystem = new RenderDebugSystem(this);
	mRenderDebugSystem->Initialize();
	
	return 0;
}

void scarlett::World::Finalize() noexcept
{
	mEntities.clear();

	mRenderDebugSystem->Finalize();
	mCameraSystem->Finalize();
	mAnimationSystem->Finalize();
	mMeshRenderSystem->Finalize();
	mTerrainSystem->Finalize();
}

void scarlett::World::Tick() noexcept
{
	mAnimationSystem->Tick();
	mMeshRenderSystem->Tick();
	mTerrainSystem->Tick();
}

void scarlett::World::Render() noexcept
{
	mCameraSystem->RenderBackground();
	mTerrainSystem->Render();
	mMeshRenderSystem->Render();
	//mRenderDebugSystem->Render();
}

std::shared_ptr<Entity> scarlett::World::CreateEntity()
{
	auto entity = std::make_shared<Entity>();
	entity->Initialize(this);
	mEntities[entity->GetGuid()] = entity;
	return entity;
}

std::shared_ptr<Entity> scarlett::World::CreateEntity(const boost::uuids::uuid & guid)
{
	if (mEntities[guid]) {
		return nullptr;
	}

	auto entity = std::make_shared<Entity>(guid);
	entity->Initialize(this);
	mEntities[guid] = entity;
	return entity;
}

void scarlett::World::DeleteEntity(const boost::uuids::uuid & guid)
{
	auto entity = mEntities[guid];
	if (entity) {
		entity->Finalize();
		mEntities.erase(guid);
	}
}

std::shared_ptr<Entity> scarlett::World::GetEntity(const boost::uuids::uuid & guid)
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
	const aiScene* scene = importer2.ReadFile(scenePath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);
	SCARLETT_ASSERT(scene);

	// load all mesh
	auto t = scene->mRootNode->mTransformation;

	for (unsigned int j = 0; j < scene->mNumAnimations; ++j) {
		auto animation = scene->mAnimations[j];
		auto name = animation->mName;
	}

	// build main camera
	auto camera = CreateEntity();
	camera->AddComponent<CameraComponent>();
	mCameraSystem->SetMainCamera(camera);

	// build scene graph entity
	auto child = scene->mRootNode->FindNode("roboto");
	auto prescale = child->mParent->mTransformation;
	auto prescale2 = child->mParent->mParent->mTransformation;
	auto prerotation = child->mParent->mParent->mParent->mTransformation;


	auto entity = CreateEntity();
	auto transformation = entity->GetComponent<TransformComponent>();
	transformation->SetPosition(Vector3f(0, 0, 0));
	transformation->SetRotation(Vector3f(0, 0, 0));
	transformation->SetScale(Vector3f(1, 1, 1));

	auto comp = entity->AddComponent<MeshRenderComponent>();
		
	for (unsigned int j = 0; j < child->mNumMeshes; ++j) {
		auto midx = child->mMeshes[j];
		auto mesh = scene->mMeshes[midx];
		auto iMesh = GApp->mGraphicsManager->CreateRenderMesh(mesh, scene);
		comp->mMeshes.push_back(iMesh);
	}

	auto armature = scene->mRootNode->FindNode("Bone001");
	if (armature) {
		auto skeleton = entity->AddComponent<SkeletonComponent>();
		skeleton->InitializeHeirarchy(armature, scene);
	}

	auto terrain = CreateEntity();
	terrain->AddComponent<TerrainComponent>();
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
		cout << "position: " <<  "(" << position.x << "," << position.y << "," << position.z << ")" << endl;
		
		auto meshRender = entity->GetComponent<MeshRenderComponent>();
		auto cameraComponent = entity->GetComponent<CameraComponent>();
		if (cameraComponent)
		{
			cout << "camera type: " << cameraComponent->GetType() << endl;
			cout << cameraComponent->GetViewMatrix() << endl;
			cout << cameraComponent->GetPerspectiveMatrix() << endl;
		}

		cout << endl;
	}
}

#include "Foundation/Assert.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/Core/Object/World.h"
#include "Runtime/Core/Math/ScltMath.h"

#include "Runtime/Core/Object/Components/MeshRenderComponent.h"
#include "Runtime/Core/Object/Components/RigidBodyComponent.h"
#include "Runtime/Core/Physics/Inertia.h"

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

	mLightSystem = new LightSystem(this);
	mLightSystem->Initialize();

	mRenderDebugSystem = new RenderDebugSystem(this);
	mRenderDebugSystem->Initialize();

	mPhysicsSystem = new PhysicsSystem(this);
	mPhysicsSystem->Initialize();

	return 0;
}

void scarlett::World::Finalize() noexcept
{
	mEntities.clear();

	mRenderDebugSystem->Finalize();
	mLightSystem->Finalize();
	mCameraSystem->Finalize();
	mAnimationSystem->Finalize();
	mMeshRenderSystem->Finalize();
	mTerrainSystem->Finalize();
}

void scarlett::World::Tick() noexcept
{
	mPhysicsSystem->Tick();
	mLightSystem->Tick();
	mAnimationSystem->Tick();
	mMeshRenderSystem->Tick();
	mTerrainSystem->Tick();
}

void scarlett::World::Render() noexcept
{
	mCameraSystem->RenderBackground();
	mCameraSystem->RenderShadowMap();

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

	// Add some box
	for(int bcount = 0; bcount < 1; bcount++)
	{
		auto entity = CreateEntity();
		auto transformation = entity->GetComponent<TransformComponent>();
		transformation->SetPosition(Vector3f(bcount * -5, 20, bcount * 5));
		transformation->SetRotation(Vector3f(0, 3.1415 / 4, 0.95538));
		transformation->SetScale(Vector3f(1, 1, 1));

		auto comp = entity->AddComponent<MeshRenderComponent>();
		auto child = scene->mRootNode->FindNode("Cube");
		auto mesh = scene->mMeshes[child->mMeshes[0]];
		auto iMesh = GApp->mGraphicsManager->CreateRenderMesh(mesh, scene);
		comp->mMeshes.push_back(iMesh);

		auto rigidBodyComp = entity->AddComponent<RigidBodyComponent>();
		float mass = 1.0f;
		Matrix3x3f inertia = Inertia::CalculateInertiaSolidCuboid(mass, 2.0f, 2.0f, 2.0f);
		float impulseCoeff = 0.5f;
		float frictionCoeff = 0.5f;
		rigidBodyComp->GetRigidBody()->InitializeCoeff(mass, inertia, impulseCoeff, frictionCoeff);
		rigidBodyComp->GetAABB()->Initialize(mesh);
		rigidBodyComp->GetRigidBody()->SetStatic(false);
	}
	
	{
		auto terrain = CreateEntity();
		auto transformation = terrain->GetComponent<TransformComponent>();
		transformation->SetPosition(Vector3f(0, -5, 0));
		transformation->SetRotation(Vector3f(0, 0, 0));
		transformation->SetScale(Vector3f(1, 1, 1));
		terrain->AddComponent<TerrainComponent>();
		auto rigidbody = terrain->AddComponent<RigidBodyComponent>();
		Vector3f minPoint = { -100.0f, -5.0f, -100.0f };
		Vector3f maxPoint = { 100.0f, 5.0f, 100.0f };
		rigidbody->GetAABB()->Initialize(minPoint, maxPoint);
		rigidbody->GetRigidBody()->SetStatic(true);
	}

	auto sun = CreateEntity();
	sun->AddComponent<LightComponent>();
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

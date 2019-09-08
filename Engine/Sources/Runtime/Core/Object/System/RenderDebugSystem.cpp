#include "RenderDebugSystem.h"
#include "Runtime/Core/Object/World.h"
#include "Runtime/Core/Application/Application.h"

scarlett::RenderDebugSystem::RenderDebugSystem(World * world):
	mWorld(world)
{
	CreateDebugMesh();
}

int scarlett::RenderDebugSystem::Initialize() noexcept
{

	return 0;
}

void scarlett::RenderDebugSystem::Finalize() noexcept
{
}

void scarlett::RenderDebugSystem::Render() noexcept {
	auto mgr = mWorld->mApp->mGraphicsManager;

	for (auto pair : mMeshes) {
		auto mesh = pair.second;
		mesh->Render(mgr, mWorld);
	}
}

void scarlett::RenderDebugSystem::CreateDebugMesh() noexcept {
	auto mgr = mWorld->mApp->mGraphicsManager;

	unsigned int pointCount = 84;
	float* data = new float[3 * pointCount];

	int p = 0;
	for (int x = -10; x <= 10; ++x) {
		data[p * 6] = x * 10;
		data[p * 6 + 1] = 0;
		data[p * 6 + 2] = -100;
		data[p * 6 + 3] = x * 10;
		data[p * 6 + 4] = 0;
		data[p * 6 + 5] = 100;
		p += 1;
	}
	for (int z = -10; z <= 10; ++z) {
		data[p * 6] = -100;
		data[p * 6 + 1] = 0;
		data[p * 6 + 2] = z * 10;
		data[p * 6 + 3] = 100;
		data[p * 6 + 4] = 0;
		data[p * 6 + 5] = z * 10;
		p += 1;
	}

	auto vertexBuffer = mgr->CreateVertexBuffer(data, pointCount, VertexFormat::VF_P3F);
	delete data;

	auto mesh = mgr->CreateRenderMesh(nullptr);
	mesh->mPositions = vertexBuffer;
	mesh->mType = PrimitiveType::PT_LINE;
	mMeshes["mesh"] = mesh;
}

void scarlett::RenderDebugSystem::DeleteDebugMesh() noexcept {
	auto mgr = mWorld->mApp->mGraphicsManager;

	auto mesh = mMeshes["mesh"];
	if (mesh) {
		mgr->DeleteRenderMesh(mesh);
		mMeshes.erase("mesh");
	}
}
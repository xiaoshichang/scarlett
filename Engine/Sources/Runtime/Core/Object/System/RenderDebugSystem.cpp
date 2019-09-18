#include "RenderDebugSystem.h"
#include "Runtime/Core/Object/World.h"
#include "Runtime/Core/Application/Application.h"

scarlett::RenderDebugSystem::RenderDebugSystem(World * world):
	mWorld(world)
{
}

int scarlett::RenderDebugSystem::Initialize() noexcept
{
	return 0;
}

void scarlett::RenderDebugSystem::Finalize() noexcept
{
	DeleteDebugMesh();
}

void scarlett::RenderDebugSystem::Render() noexcept {
	auto mgr = mWorld->mApp->mGraphicsManager;

	for (auto pair : mMeshes) {
		auto mesh = pair.second;
		auto worldMatrix = Matrix4f::Identity();
		mesh->Render(mWorld, worldMatrix);
	}
}

void scarlett::RenderDebugSystem::CreateDebugMesh() noexcept {
	auto mgr = mWorld->mApp->mGraphicsManager;

	unsigned int pointCount = 80;
	float* data = new float[3 * pointCount];

	int p = 0;
	for (int x = -10; x <= 10; ++x) {
		if (x == 0) continue;
		data[p * 6] = x;
		data[p * 6 + 1] = 0;
		data[p * 6 + 2] = -10;
		data[p * 6 + 3] = x;
		data[p * 6 + 4] = 0;
		data[p * 6 + 5] = 10;
		p += 1;
	}
	for (int z = -10; z <= 10; ++z) {
		if (z == 0) continue;
		data[p * 6] = -10;
		data[p * 6 + 1] = 0;
		data[p * 6 + 2] = z;
		data[p * 6 + 3] = 10;
		data[p * 6 + 4] = 0;
		data[p * 6 + 5] = z;
		p += 1;
	}

	auto mesh = mgr->CreateRenderMeshDebug(data, pointCount, VertexFormat::VF_P3F);
	mesh->mType = PrimitiveType::PT_LINE;
	mesh->mMaterial->SetShaderParamter("color", Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
	mMeshes["mesh"] = mesh;
	delete data;

}

void scarlett::RenderDebugSystem::DeleteDebugMesh() noexcept {
	auto mgr = mWorld->mApp->mGraphicsManager;

	auto mesh = mMeshes["mesh"];
	if (mesh) {
		mMeshes.erase("mesh");
	}
}

void scarlett::RenderDebugSystem::CreateDebugAxis() noexcept
{
	auto mgr = mWorld->mApp->mGraphicsManager;
	float* x = new float[3 * 2];
	x[0] = 1000.0f;
	x[1] = 0.0f;
	x[2] = 0.0f;
	x[3] = -1000.0f;
	x[4] = 0.0f;
	x[5] = 0.0f;
	auto mesh = mgr->CreateRenderMeshDebug(x, 2, VertexFormat::VF_P3F);
	mesh->mType = PrimitiveType::PT_LINE;
	mesh->mMaterial->SetShaderParamter("color", Vector4f(1.0f, 0.0f, 0.0f, 1.0f));
	mMeshes["x"] = mesh;

	float* y = new float[3 * 2];
	y[0] = 0.0f;
	y[1] = 1000.0f;
	y[2] = 0.0f;
	y[3] = 0.0f;
	y[4] = -1000.0f;
	y[5] = 0.0f;
	auto meshy = mgr->CreateRenderMeshDebug(y, 2, VertexFormat::VF_P3F);
	meshy->mType = PrimitiveType::PT_LINE;
	meshy->mMaterial->SetShaderParamter("color", Vector4f(0.0f, 1.0f, 0.0f, 1.0f));
	mMeshes["y"] = meshy;

	float* z = new float[3 * 2];
	z[0] = 0.0f;
	z[1] = 0.0f;
	z[2] = 1000.0f;
	z[3] = 0.0f;
	z[4] = 0.0f;
	z[5] = -1000.0f;
	auto meshz = mgr->CreateRenderMeshDebug(z, 2, VertexFormat::VF_P3F);
	meshz->mType = PrimitiveType::PT_LINE;
	meshz->mMaterial->SetShaderParamter("color", Vector4f(0.0f, 0.0f, 1.0f, 1.0f));
	mMeshes["z"] = meshz;

	delete z;
	delete y;
	delete x;

}

void scarlett::RenderDebugSystem::DeleteDebugAxis() noexcept
{
	auto mgr = mWorld->mApp->mGraphicsManager;
	auto meshx = mMeshes["x"];
	if (meshx) {
		mMeshes.erase("x");
	}
	auto meshy = mMeshes["y"];
	if (meshy) {
		mMeshes.erase("y");
	}
	auto meshz = mMeshes["z"];
	if (meshz) {
		mMeshes.erase("z");
	}
}

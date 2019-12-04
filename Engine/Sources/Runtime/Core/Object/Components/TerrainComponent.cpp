#include "TerrainComponent.h"
#include "Runtime/Core/Object/Entity.h"
#include "Runtime/Core/Object/World.h"
#include "Runtime/Core/Object/Components/CameraComponent.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/Core/Memory/MemoryManager.h"
#include "Runtime/Utils/Logging.h"


scarlett::TerrainUnit::TerrainUnit():
	mVisible(true), mLod(TerrainLod::Low)
{
}

scarlett::TerrainUnit::~TerrainUnit()
{
	FinalizeRenderResource();
}

void scarlett::TerrainUnit::UpdateVisibility(CameraComponent* camera)
{
	auto campos = camera->GetPosition();
	campos.y = 0;
	auto dis = Distance(campos, mOffset);
	if (dis < 50000.0f) {
		mLod = TerrainLod::High;
	}
	else {
		mLod = TerrainLod::Low;
	}
}

void scarlett::TerrainUnit::UpdateLod(CameraComponent* camera)
{
	mVisible = true;
}

void scarlett::TerrainUnit::SetOffset(const Vector3f& offset)
{
	mOffset = offset;
}


void scarlett::TerrainUnit::InitalizeRenderResourceHigh()
{
	int vertex;

	int count = 11;	// vertex count
	float gridSize = 100.0f / (count-1);

	int idxStart = -(count - 1) / 2;
	int idxEnd = (count - 1) / 2;

	// init positions
	int vertexCount = count * count;
	int vertexBufferSize = vertexCount * sizeof(float) * 3;
	auto positions = (float*)GMemoryManager::GetInstance()->Allocate(vertexBufferSize);

	vertex = 0;
	for (int i = idxStart; i <= idxEnd; ++i) {
		for (int j = idxStart; j <= idxEnd; ++j) {
			positions[vertex * 3] = i * gridSize + mOffset.x;
			positions[vertex * 3 + 1] = 0 + mOffset.y;
			positions[vertex * 3 + 2] = j * gridSize + mOffset.z;
			vertex += 1;
		}
	}
	auto positionBuffer = GApp->mGraphicsManager->CreateVertexBuffer(positions, vertexCount, VertexFormat::VF_P3F);

	// init uv
	int uvBufferSize = vertexCount * sizeof(float) * 2;
	auto uvs = (float*)GMemoryManager::GetInstance()->Allocate(uvBufferSize);
	vertex = 0;
	float deltau = 1.0f / (count - 1);
	float deltav = 1.0f / (count - 1);
	for (int i = 0; i < count; ++i) {
		for (int j = 0; j < count; ++j) {
			uvs[vertex * 2] = deltau * j;
			uvs[vertex * 2 + 1] = deltav * i;
			vertex += 1;
		}
	}
	auto uvBuffer = GApp->mGraphicsManager->CreateVertexBuffer(uvs, vertexCount, VertexFormat::VF_T2F);
	
	// init index buffer
	int gridCount = (count - 1);
	int indexCount = gridCount * gridCount * 6;
	int indexBufferSize = indexCount * sizeof(unsigned int);
	auto indexes = (unsigned int*)GMemoryManager::GetInstance()->Allocate(indexBufferSize);

	int grid = 0;
	for (int i = 0; i < gridCount; ++i) {
		for (int j = 0; j < gridCount; ++j) {
			indexes[grid * 6] = i * count + j;
			indexes[grid * 6 + 1] = i * count + j + 1;
			indexes[grid * 6 + 2] = (i + 1)*count + j;
			indexes[grid * 6 + 3] = (i + 1)*count + j;
			indexes[grid * 6 + 4] = i * count + j + 1;
			indexes[grid * 6 + 5] = (i + 1)*count + j + 1;
			grid += 1;
		}
	}
	auto indexBuffer = GApp->mGraphicsManager->CreateIndexBuffer(indexes, indexCount, IndexFormat::IF_UINT32);

	mMeshHigh = GApp->mGraphicsManager->CreateRenderMesh();
	mMeshHigh->mMeshType = MeshType::MT_TERRAIN;
	mMeshHigh->mPositions = positionBuffer;
	mMeshHigh->mTexCoords = uvBuffer;
	mMeshHigh->mIndexes = indexBuffer;
	mMeshHigh->InitializeTerrain();

	auto shader = GApp->mGraphicsManager->GetShader("terrain_high");
	mMeshHigh->mMaterial->SetShader(shader);
	mMeshHigh->mMaterial->SetShaderParamter("color", Vector4f(0.0f, 0.5f, 0.0f, 1.0f));

	auto tex = GApp->mGraphicsManager->CreateTexture2D("./Asset/Textures/terrain_high.png");
	mMeshHigh->mMaterial->SetTexture("terrain", tex);

	auto smaplerState = GApp->mGraphicsManager->CreateSamplerState();
	mMeshHigh->mMaterial->SetSamplerState("terrain", smaplerState);

	GMemoryManager::GetInstance()->Free(positions, vertexBufferSize);
	GMemoryManager::GetInstance()->Free(uvs, uvBufferSize);
	GMemoryManager::GetInstance()->Free(indexes, indexCount);
}

void scarlett::TerrainUnit::InitalizeRenderResourceLow()
{
	int count = 3;	// vertex count
	float gridSize = 100.0f / (count - 1);

	int idxStart = -(count - 1) / 2;
	int idxEnd = (count - 1) / 2;

	// init positions
	int vertexCount = count * count;
	int vertexBufferSize = vertexCount * sizeof(float) * 3;
	auto positions = (float*)GMemoryManager::GetInstance()->Allocate(vertexBufferSize);

	int vertex = 0;
	for (int i = idxStart; i <= idxEnd; ++i) {
		for (int j = idxStart; j <= idxEnd; ++j) {
			positions[vertex * 3] = i * gridSize + mOffset.x;
			positions[vertex * 3 + 1] = 0 + mOffset.y;
			positions[vertex * 3 + 2] = j * gridSize + mOffset.z;
			vertex += 1;
		}
	}

	auto positionBuffer = GApp->mGraphicsManager->CreateVertexBuffer(positions, vertexCount, VertexFormat::VF_P3F);

	// init uv
	int uvBufferSize = vertexCount * sizeof(float) * 2;
	auto uvs = (float*)GMemoryManager::GetInstance()->Allocate(uvBufferSize);
	vertex = 0;
	float deltau = 1.0f / (count - 1);
	float deltav = 1.0f / (count - 1);
	for (int i = 0; i < count; ++i) {
		for (int j = 0; j < count; ++j) {
			uvs[vertex * 2] = deltau * j;
			uvs[vertex * 2 + 1] = deltav * i;
			vertex += 1;
		}
	}
	auto uvBuffer = GApp->mGraphicsManager->CreateVertexBuffer(uvs, vertexCount, VertexFormat::VF_T2F);

	// init index buffer
	int gridCount = (count - 1);
	int indexCount = gridCount * gridCount * 6;
	int indexBufferSize = indexCount * sizeof(unsigned int);
	auto indexes = (unsigned int*)GMemoryManager::GetInstance()->Allocate(indexBufferSize);

	int grid = 0;
	for (int i = 0; i < gridCount; ++i) {
		for (int j = 0; j < gridCount; ++j) {
			indexes[grid * 6] = i * count + j;
			indexes[grid * 6 + 1] = i * count + j + 1;
			indexes[grid * 6 + 2] = (i + 1)*count + j;
			indexes[grid * 6 + 3] = (i + 1)*count + j;
			indexes[grid * 6 + 4] = i * count + j + 1;
			indexes[grid * 6 + 5] = (i + 1)*count + j + 1;
			grid += 1;
		}
	}
	auto indexBuffer = GApp->mGraphicsManager->CreateIndexBuffer(indexes, indexCount, IndexFormat::IF_UINT32);

	mMeshLow = GApp->mGraphicsManager->CreateRenderMesh();
	mMeshLow->mMeshType = MeshType::MT_TERRAIN;
	mMeshLow->mPositions = positionBuffer;
	mMeshLow->mTexCoords = uvBuffer;
	mMeshLow->mIndexes = indexBuffer;
	mMeshLow->InitializeTerrain();

	auto shader = GApp->mGraphicsManager->GetShader("terrain_low");
	mMeshLow->mMaterial->SetShader(shader);
	mMeshLow->mMaterial->SetShaderParamter("color", Vector4f(0.5f, 0.0f, 0.0f, 1.0f));
	auto tex = GApp->mGraphicsManager->CreateTexture2D("./Asset/Textures/terrain_low.png");
	mMeshLow->mMaterial->SetTexture("terrain", tex);
	
	auto smaplerState = GApp->mGraphicsManager->CreateSamplerState();
	mMeshLow->mMaterial->SetSamplerState("terrain", smaplerState);

	GMemoryManager::GetInstance()->Free(positions, vertexBufferSize);
	GMemoryManager::GetInstance()->Free(indexes, indexCount);
	GMemoryManager::GetInstance()->Free(uvs, uvBufferSize);
}

void scarlett::TerrainUnit::InitializeRenderResource()
{
	// terrain  has different highmap, every unit has different data.
	// so initialize data in every unit
	InitalizeRenderResourceHigh();
	InitalizeRenderResourceLow();
}

void scarlett::TerrainUnit::FinalizeRenderResource()
{
}



//////////////////// TerrainComponent //////////////////

scarlett::TerrainComponent::TerrainComponent()
{
}

scarlett::TerrainComponent::~TerrainComponent()
{
}

int scarlett::TerrainComponent::Initialize() noexcept
{
	auto master = GetMaster();
	if (master) {
		auto world = master->GetWorld();
		if (world) {
			world->GetTerrainSystem()->AddComponent(this);
		}
	}
	LoadResource();
	return 0;
}

void scarlett::TerrainComponent::Finalize() noexcept
{
	auto master = GetMaster();
	if (master) {
		auto world = master->GetWorld();
		if (world) {
			world->GetTerrainSystem()->DeleteComponent(this);
		}
	}
}

void scarlett::TerrainComponent::Tick() noexcept
{
	auto master = GetMaster();
	if (master) {
		auto world = master->GetWorld();
		if (world) {
			auto camera = world->GetCameraSystem()->GetMainCamera()->GetComponent<CameraComponent>();
			UpdateVisibility(camera);
			UpdateLod(camera);
		}
	}
}

void scarlett::TerrainComponent::LoadResource()
{
	int unitRow = 10;
	int unitCol = 10;
	float gridSize = 100.0f;
	float hgridSize = gridSize / 2;

	for (int r = 0; r < unitRow; ++r) {
		for (int c = 0; c < unitCol; ++c) {
			int rowIdx = r - unitRow / 2;
			int colIdx = c - unitCol / 2;
			float x = hgridSize + rowIdx * gridSize;
			float y = 0.0f;
			float z = hgridSize + colIdx * gridSize;
			auto position = Vector3f(x, y, z);

			auto unit = std::make_shared<TerrainUnit>();
			unit->SetOffset(position);
			unit->InitializeRenderResource();
			mUnits.push_back(unit);
		}
	}
}

// update terrain units visibility by camera
void scarlett::TerrainComponent::UpdateVisibility(CameraComponent* camera)
{
	for (auto unit : mUnits) {
		unit->UpdateVisibility(camera);
	}
}

// upadte terrain units lod by camrea
void scarlett::TerrainComponent::UpdateLod(CameraComponent* camera)
{
	for (auto unit : mUnits) {
		if (unit->mVisible) {
			unit->UpdateLod(camera);
		}
	}
}

void scarlett::TerrainComponent::Render()
{
	for (auto unit : mUnits) {
		if (unit->mVisible) {
			auto entity = GetMaster();
			if (unit->mLod == TerrainLod::High) {
				unit->mMeshHigh->Render(entity);
			}
			else
			{
				unit->mMeshLow->Render(entity);
			}
		}
	}
}

#include "TerrainComponent.h"
#include "Runtime/Core/Object/Entity.h"
#include "Runtime/Core/Object/World.h"
#include "Runtime/Core/Object/Components/CameraComponent.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/Core/Memory/MemoryManager.h"
#include "Runtime/Utils/Logging.h"


scarlett::TerrainUnit::TerrainUnit(TerrainComponent* master):
	mVisible(true), mLod(TerrainLod::Low)
{
	mMaster = master;
}

scarlett::TerrainUnit::~TerrainUnit()
{
	FinalizeRenderResource();
}

void scarlett::TerrainUnit::UpdateVisibility(CameraComponent* camera)
{
	// todo: use Frustum Culling
	mVisible = true;
}

void scarlett::TerrainUnit::UpdateLod(CameraComponent* camera)
{
	auto campos = camera->GetPosition();
	campos.y = 0;
	auto dis = Distance(campos, mOffset);
	if (dis < 200) {
		mLod = TerrainLod::High;
	}
	else {
		mLod = TerrainLod::Low;
	}
}

void scarlett::TerrainUnit::SetOffset(const Vector3f& offset)
{
	mOffset = offset;
}


void scarlett::TerrainUnit::InitalizeRenderResourceHigh()
{
	int vertex;

	int count = 33;	// vertex count
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
			float x = i * gridSize + mOffset.x;
			float z = j * gridSize + mOffset.z;
			positions[vertex * 3] = x;
			positions[vertex * 3 + 2] = z;
			float height = GetHeight(x, z);
			positions[vertex * 3 + 1] = mOffset.y + height;

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
	int count = 33;	// vertex count
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
			float x = i * gridSize + mOffset.x;
			float z = j * gridSize + mOffset.z;
			positions[vertex * 3] = x;
			positions[vertex * 3 + 2] = z;
			positions[vertex * 3 + 1] = mOffset.y + GetHeight(x, z);
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

/*
get vertex height by x and z coord and heightmap.
*/
float scarlett::TerrainUnit::GetHeight(float x, float z) {
	
	float xmin = -mMaster->mUnitCol / 2.0 * mMaster->mUnitSize;
	float zmin = -mMaster->mUnitRow / 2.0 * mMaster->mUnitSize;

	float u = (x - xmin) / (fabsf(xmin) * 2);
	float v = (z - zmin) / (fabsf(zmin) * 2);

	int uidx = int(u * (mMaster->mHeightMapWidth - 1));
	int vidx = int(v * (mMaster->mHeightMapHeight - 1));

	int idx = uidx * mMaster->mHeightMapWidth + vidx;
	unsigned char r = mMaster->mHeightMapData[idx * 4];
	unsigned char g = mMaster->mHeightMapData[idx * 4 + 1];
	unsigned char b = mMaster->mHeightMapData[idx * 4 + 2];

	float rf = r / 255.0f;
	float gf = g / 255.0f;
	float bf = b / 255.0f;

	float h = rf * 0.299 + gf * 0.587 + bf * 0.144;
	return h * 100 - 10;
}

//////////////////// TerrainComponent //////////////////

scarlett::TerrainComponent::TerrainComponent()
{
	mHeightMapData = 0;
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

	if (mHeightMapData) {
		scarlett::ImageParser::Free(mHeightMapData);
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
	LoadHeightMap();

	mUnitCol = 10;
	mUnitRow = 10;
	mUnitSize = 100.0f;
	float hgridSize = mUnitSize / 2;

	for (int r = 0; r < mUnitRow; ++r) {
		for (int c = 0; c < mUnitCol; ++c) {
			int rowIdx = r - mUnitRow / 2;
			int colIdx = c - mUnitCol / 2;
			float x = hgridSize + rowIdx * mUnitSize;
			float y = 0.0f;
			float z = hgridSize + colIdx * mUnitSize;
			auto position = Vector3f(x, y, z);

			auto unit = std::make_shared<TerrainUnit>(this);
			unit->SetOffset(position);
			unit->InitializeRenderResource();
			mUnits.push_back(unit);
		}
	}
}

void scarlett::TerrainComponent::LoadHeightMap()
{
	std::string filepath = "./Asset/Textures/highmap.jpg";
	mHeightMapData = scarlett::ImageParser::Parse(filepath, &mHeightMapWidth, &mHeightMapHeight, &mHeightMapChannels, 4);
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

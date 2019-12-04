#include "Runtime/RHI/D11/MeshD11.h"
#include "Runtime/RHI/D11/IndexBufferD11.h"
#include "Runtime/RHI/D11/VertexBufferD11.h"
#include "Runtime/RHI/D11/MaterialD11.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Runtime/Core/Object/World.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/Core/Math/ScltMath.h"
#include "Foundation/Assert.h"


using namespace scarlett;

void AppendBoneWeight(int* idxes, float* weights, int vidx, int bidx, float weight) {
	for (int offset = 0; offset < 4; offset++) {
		if (idxes[vidx * 4 + offset] == -1) {
			idxes[vidx * 4 + offset] = bidx;
			weights[vidx * 4 + offset] = weight;
			return;
		}
	}
	SCARLETT_ASSERT(false);
}

scarlett::MeshD11::MeshD11()
{
	mMaterial = std::make_shared<MaterialD11>();
}

scarlett::MeshD11::MeshD11(aiMesh* mesh, const aiScene* world)
{
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	Initialize(mesh);

	// hardcode material
	auto material = world->mMaterials[mesh->mMaterialIndex];
	aiString name;
	aiGetMaterialString(material, AI_MATKEY_NAME, &name);

	mMaterial = std::make_shared<MaterialD11>();
	aiColor4D diffuse;
	aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
	auto shader = mgrd11->GetShader("pbr_skin");
	mMaterial->SetShader(shader);

	float roughness = 0.1f;
	float metallic = 0.8f;
	mMaterial->SetShaderParamter("color", Vector4f(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
	mMaterial->SetShaderParamter("pbrParameter", Vector4f(roughness, metallic, 0.0f, 0.0f));

	auto lut = mgrd11->CreateTexture2D("./Asset/Textures/ibl_brdf_lut.png");
	auto lutSampler = mgrd11->CreateSamplerState();
	mMaterial->SetTexture("lut", lut);
	mMaterial->SetSamplerState("lut", lutSampler);
}

scarlett::MeshD11::MeshD11(void* data, int count, VertexFormat vf)
{
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	Initialize(data, count, vf);

	// hardcode material
	mMaterial = std::make_shared<MaterialD11>();
	auto shader = mgrd11->GetShader("debug");
	mMaterial->SetShader(shader);
}

scarlett::MeshD11::~MeshD11()
{
	delete stride;
	delete offset;
	delete vbuffers;
	mPositions = nullptr;
	mNormals = nullptr;
	mTexCoords = nullptr;
	mIndexes = nullptr;
}

void scarlett::MeshD11::Initialize(aiMesh * mesh) noexcept
{
	if (!mesh) {
		return;
	}
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;

	auto count = mesh->mNumVertices;
	if (mesh->HasPositions()) {
		mPositions = mgrd11->CreateVertexBuffer(mesh->mVertices, count, VertexFormat::VF_P3F);
	}

	if (mesh->HasNormals()) {
		mNormals = mgrd11->CreateVertexBuffer(mesh->mNormals, count, VertexFormat::VF_N3F);
	}

	if (mesh->HasTextureCoords(0)) {
		float *texCoords = (float *)malloc(sizeof(float) * 2 * mesh->mNumVertices);
		for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
			texCoords[k * 2] = mesh->mTextureCoords[0][k].x;
			texCoords[k * 2 + 1] = mesh->mTextureCoords[0][k].y;
		}
		mTexCoords = mgrd11->CreateVertexBuffer(texCoords, count, VertexFormat::VF_T2F);
		free(texCoords);
	}

	if (mesh->HasBones()) {
		int* boneIdx = (int*)malloc(sizeof(int) * 4 * mesh->mNumVertices);
		float* weights = (float*)malloc(sizeof(float) * 4 * mesh->mNumVertices);

		memset(boneIdx, -1, sizeof(int) * 4 * mesh->mNumVertices);
		memset(weights, 0, sizeof(float) * 4 * mesh->mNumVertices);
		
		for (int b = 0; b < mesh->mNumBones; b++) {
			auto bone = mesh->mBones[b];

			for (int w = 0; w < bone->mNumWeights; ++w) {
				auto weight = bone->mWeights[w];
				auto vidx = weight.mVertexId;
				auto vw = weight.mWeight;
				AppendBoneWeight(boneIdx, weights, vidx, b, vw);
			}
		}

		for (int i = 0; i < 4 * mesh->mNumVertices; i++) {
			if (boneIdx[i] == -1) {
				boneIdx[i] = 0;
			}
		}

		mBoneIdxes = mgrd11->CreateVertexBuffer(boneIdx, count, VertexFormat::VF_BONE_IDX_4I);
		mBoneWeights = mgrd11->CreateVertexBuffer(weights, count, VertexFormat::VF_BONE_WEIGHT_4F);

		free(boneIdx);
		free(weights);
	}


	if (mesh->HasFaces()) {
		unsigned int count = 3 * mesh->mNumFaces;
		unsigned int * idata = new unsigned int[count];
		for (int i = 0; i < mesh->mNumFaces; ++i) {
			auto face = mesh->mFaces[i];
			idata[i * 3] = face.mIndices[0];
			idata[i * 3 + 1] = face.mIndices[1];
			idata[i * 3 + 2] = face.mIndices[2];
		}
		mIndexes = mgrd11->CreateIndexBuffer(idata, count, IndexFormat::IF_UINT32);
		free(idata);
	}

	if (mesh->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_POINT) {
		mPrimitiveType = PrimitiveType::PT_POINT;
	}
	else if (mesh->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_LINE) {
		mPrimitiveType = PrimitiveType::PT_LINE;
	}
	else if (mesh->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_TRIANGLE)
	{
		mPrimitiveType = PrimitiveType::PT_TRIANGLE;
	}
	else {
		SCARLETT_ASSERT(false);
	}
	InitializeStrideOffset();
}

void scarlett::MeshD11::Initialize(void * data, int count, VertexFormat vf) noexcept
{
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	mPositions = mgrd11->CreateVertexBuffer(data, count, vf);
	mPrimitiveType = PrimitiveType::PT_LINE;
	vbcount = 1;
	stride = new  unsigned int[vbcount];
	offset = new  unsigned int[vbcount];
	vbuffers = new ID3D11Buffer *[vbcount];

	VertexBufferD11* buffer;
	stride[0] = mPositions->GetVertexSize(mPositions->mVertexFormat);
	offset[0] = 0;
	buffer = (VertexBufferD11*)mPositions.get();
	vbuffers[0] = buffer->mVertexBuffer;
}

void scarlett::MeshD11::InitializeUI() noexcept
{
	mMeshType = MeshType::MT_UI;

	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	mPrimitiveType = PrimitiveType::PT_TRIANGLE;

	vbcount = 2;
	float position[18] = {-1, 1, 0, 1, -1, 0, -1, -1, 0, -1, 1, 0, 1, 1, 0, 1, -1, 0};
	float texcood[12] = {0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1};

	mPositions = mgrd11->CreateVertexBuffer(position, 6, VertexFormat::VF_P3F);
	mTexCoords = mgrd11->CreateVertexBuffer(texcood, 6, VertexFormat::VF_T2F);

	InitializeStrideOffset();

	mMaterial = make_shared<MaterialD11>();
	auto shader = mgrd11->GetShader("ui");
	mMaterial->SetShader(shader);
	auto sampler = mgrd11->CreateSamplerState();
	mMaterial->SetSamplerState("ui", sampler);
}

void scarlett::MeshD11::InitializeTerrain() noexcept
{
	mPrimitiveType = PrimitiveType::PT_TRIANGLE;
	InitializeStrideOffset();
}

void scarlett::MeshD11::InitializeStrideOffset()
{
	vbcount = 5;
	stride = new  unsigned int[vbcount];
	offset = new  unsigned int[vbcount];
	vbuffers = new ID3D11Buffer *[vbcount];
	VertexBufferD11* buffer;
	int idx = 0;
	if (mPositions) {
		stride[idx] = mPositions->GetVertexSize(mPositions->mVertexFormat);
		buffer = (VertexBufferD11*)mPositions.get();
		vbuffers[idx] = buffer->mVertexBuffer;
	}
	else {
		stride[idx] = 0;
		vbuffers[idx] = nullptr;
	}
	offset[idx] = 0;
	idx += 1;


	if (mNormals) {
		stride[idx] = mNormals->GetVertexSize(mNormals->mVertexFormat);
		buffer = (VertexBufferD11*)mNormals.get();
		vbuffers[idx] = buffer->mVertexBuffer;
	}
	else {
		stride[idx] = 0;
		vbuffers[idx] = nullptr;
	}
	offset[idx] = 0;
	idx += 1;

	if (mTexCoords) {
		stride[idx] = mTexCoords->GetVertexSize(mTexCoords->mVertexFormat);
		buffer = (VertexBufferD11*)mTexCoords.get();
		vbuffers[idx] = buffer->mVertexBuffer;
	}
	else {
		stride[idx] = 0;
		vbuffers[idx] = nullptr;
	}
	offset[idx] = 0;
	idx += 1;


	if (mBoneIdxes) {
		stride[idx] = mBoneIdxes->GetVertexSize(mBoneIdxes->mVertexFormat);
		buffer = (VertexBufferD11*)mBoneIdxes.get();
		vbuffers[idx] = buffer->mVertexBuffer;
	}
	else {
		stride[idx] = 0;
		vbuffers[idx] = nullptr;
	}
	offset[idx] = 0;
	idx += 1;

	if (mBoneWeights) {
		stride[idx] = mBoneWeights->GetVertexSize(mBoneWeights->mVertexFormat);
		buffer = (VertexBufferD11*)mBoneWeights.get();
		vbuffers[idx] = buffer->mVertexBuffer;
	}
	else {
		stride[idx] = 0;
		vbuffers[idx] = nullptr;
	}
	offset[idx] = 0;
	idx += 1;
}

void scarlett::MeshD11::Render(Entity* self) noexcept
{
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	mgrd11->m_deviceContext->IASetVertexBuffers(0, vbcount, vbuffers, stride, offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	if (mIndexes) {
		auto indexBuffer = static_pointer_cast<IndexBufferD11>(mIndexes);
		mgrd11->m_deviceContext->IASetIndexBuffer(indexBuffer->mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	if (mPrimitiveType == PrimitiveType::PT_LINE) {
		mgrd11->m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	}
	else if (mPrimitiveType == PrimitiveType::PT_POINT) {
		mgrd11->m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	}
	else if (mPrimitiveType == PrimitiveType::PT_TRIANGLE) {
		mgrd11->m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	ConstantBuffer cb;
	if (mMeshType == MT_Skybox) {
		BuildMatrixIdentity(cb.world);
		auto world = self->GetWorld();
		auto camera = world->GetCameraSystem()->GetMainCamera()->GetComponent<CameraComponent>();
		cb.view = camera->GetViewMatrixOrigin();
		cb.projection = camera->GetPerspectiveMatrix();
	}
	else if (mMeshType == MT_Model) {
		cb.world = self->GetComponent<TransformComponent>()->GetWorldMatrix();
		auto world = self->GetWorld();
		auto camera = world->GetCameraSystem()->GetMainCamera()->GetComponent<CameraComponent>();
		cb.view = camera->GetViewMatrix();
		cb.projection = camera->GetPerspectiveMatrix();
	}
	else if (mMeshType == MT_TERRAIN) {
		cb.world = BuildMatrixTranslation(0, 0, 0);
		auto world = self->GetWorld();
		auto camera = world->GetCameraSystem()->GetMainCamera()->GetComponent<CameraComponent>();
		cb.view = camera->GetViewMatrix();
		cb.projection = camera->GetPerspectiveMatrix();
	}


	auto skeleton = self->GetComponent<SkeletonComponent>();
	if (skeleton) {
		for (int i = 0; i < 32; i++) {
			cb.boneMatrix[i] = skeleton->mSkeleton->mBoneTransforms[i];
		}
	}

	mMaterial->Apply(cb);

	if (mIndexes) {
		mgrd11->DrawIndexed(mIndexes->GetIndexCount(), 0, 0);
	}
	else {
		mgrd11->Draw(mPositions->GetVertextCount(), 0);
	}

}

void scarlett::MeshD11::Render(const Matrix4x4f& world, const Matrix4x4f& view, const Matrix4x4f& projection) noexcept
{
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	mgrd11->m_deviceContext->IASetVertexBuffers(0, vbcount, vbuffers, stride, offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	if (mIndexes) {
		auto indexBuffer = static_pointer_cast<IndexBufferD11>(mIndexes);
		mgrd11->m_deviceContext->IASetIndexBuffer(indexBuffer->mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	if (mPrimitiveType == PrimitiveType::PT_LINE) {
		mgrd11->m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	}
	else if (mPrimitiveType == PrimitiveType::PT_POINT) {
		mgrd11->m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	}
	else if (mPrimitiveType == PrimitiveType::PT_TRIANGLE) {
		mgrd11->m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	
	ConstantBuffer cb;
	cb.world = world;
	cb.view = view;
	cb.projection = projection;

	mMaterial->Apply(cb);

	if (mIndexes) {
		mgrd11->DrawIndexed(mIndexes->GetIndexCount(), 0, 0);
	}
	else {
		mgrd11->Draw(mPositions->GetVertextCount(), 0);
	}
}

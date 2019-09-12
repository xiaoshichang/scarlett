#include "Runtime/RHI/D11/RenderMeshD11.h"
#include "Runtime/RHI/D11/IndexBufferD11.h"
#include "Runtime/RHI/D11/VertexBufferD11.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Runtime/Core/Object/World.h"
#include "Runtime/Core/Application/Application.h"
#include "Foundation/Assert.h"

scarlett::RenderMeshD11::RenderMeshD11(aiMesh* mesh)
{
	Initialize(mesh);
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	mMaterial = std::make_shared<Material>();
	auto shader = mgrd11->GetShader("debug");
	mMaterial->SetShader(shader);

}

scarlett::RenderMeshD11::RenderMeshD11(std::shared_ptr<VertexBuffer> vb)
{
	Initialize(vb);
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	mMaterial = std::make_shared<Material>();
	auto shader = mgrd11->GetShader("debug");
	mMaterial->SetShader(shader);
}

scarlett::RenderMeshD11::~RenderMeshD11()
{
	Finialize();
}

void scarlett::RenderMeshD11::Initialize(aiMesh * mesh) noexcept
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
		delete texCoords;
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
		delete idata;
	}

	if (mesh->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_POINT) {
		mType = PrimitiveType::PT_POINT;
	}
	else if (mesh->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_LINE) {
		mType = PrimitiveType::PT_LINE;
	}
	else if (mesh->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_TRIANGLE)
	{
		mType = PrimitiveType::PT_TRIANGLE;
	}
	else {
		SCARLETT_ASSERT(false);
	}

	vbcount = GetVaildVertexBufferCount();
	stride = new  unsigned int[vbcount];
	offset = new  unsigned int[vbcount];
	vbuffers = new ID3D11Buffer *[vbcount];
	VertexBufferD11* buffer;
	int idx = 0;
	if (mPositions) {
		stride[idx] = mPositions->GetVertexSize(mPositions->mVertexFormat);
		offset[idx] = 0;
		buffer = (VertexBufferD11*)mPositions.get();
		vbuffers[idx] = buffer->mVertexBuffer;
		idx += 1;
	}
	if (mNormals) {
		stride[idx] = mNormals->GetVertexSize(mNormals->mVertexFormat);
		offset[idx] = 0;
		buffer = (VertexBufferD11*)mNormals.get();
		vbuffers[idx] = buffer->mVertexBuffer;
		idx += 1;
	}
	if (mTexCoords) {
		stride[idx] = mTexCoords->GetVertexSize(mTexCoords->mVertexFormat);
		offset[idx] = 0;
		buffer = (VertexBufferD11*)mTexCoords.get();
		vbuffers[idx] = buffer->mVertexBuffer;
		idx += 1;
	}
}

void scarlett::RenderMeshD11::Initialize(std::shared_ptr<VertexBuffer> vb) noexcept
{
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	mPositions = vb;
	mType = PrimitiveType::PT_LINE;
	vbcount = GetVaildVertexBufferCount();
	stride = new  unsigned int[vbcount];
	offset = new  unsigned int[vbcount];
	vbuffers = new ID3D11Buffer *[vbcount];
	VertexBufferD11* buffer;
	int idx = 0;
	if (mPositions) {
		stride[idx] = mPositions->GetVertexSize(mPositions->mVertexFormat);
		offset[idx] = 0;
		buffer = (VertexBufferD11*)mPositions.get();
		vbuffers[idx] = buffer->mVertexBuffer;
		idx += 1;
	}
}

void scarlett::RenderMeshD11::Render(World* world, const Matrix4f& worldMatrix) noexcept
{
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	mgrd11->m_deviceContext->IASetVertexBuffers(0, vbcount, vbuffers, stride, offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	if (mIndexes) {
		auto indexBuffer = static_pointer_cast<IndexBufferD11>(mIndexes);
		mgrd11->m_deviceContext->IASetIndexBuffer(indexBuffer->mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	if (mType == PrimitiveType::PT_LINE) {
		mgrd11->m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	}
	else if (mType == PrimitiveType::PT_POINT) {
		mgrd11->m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	}
	else if (mType == PrimitiveType::PT_TRIANGLE) {
		mgrd11->m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	
	auto shader = mMaterial->GetShader();
	shader->Use();
	auto camera = world->GetCameraSystem()->GetMainCamera()->GetComponent<CameraComponent>();

	ConstantBuffer cb;
	cb.world = worldMatrix.transpose();
	cb.view = camera->GetViewMatrix().transpose();
	cb.projection = camera->GetPerspectiveMatrix().transpose();
	shader->SetConstantBuffer(cb);

	if (mIndexes) {
		mgrd11->DrawIndexed(mIndexes->mCount, 0, 0);
	}
	else {
		mgrd11->Draw(mPositions->mCount, 0);
	}

}

void scarlett::RenderMeshD11::Finialize() noexcept
{
	mPositions = nullptr;
	mNormals = nullptr;
	mTexCoords = nullptr;
	mIndexes = nullptr;
}

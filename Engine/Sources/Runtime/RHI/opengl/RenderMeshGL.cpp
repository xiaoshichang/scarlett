#include "Runtime/RHI/opengl/RenderMeshGL.h"
#include "Runtime/RHI/opengl/VertexBufferGL.h"
#include "Runtime/RHI/opengl/IndexBufferGL.h"
#include "Runtime/RHI/opengl/MaterialGL.h"
#include "Runtime/RHI/opengl/GraphicsMgrGL.h"
#include "Runtime/Core/Application/Application.h"

#include "Foundation/Assert.h"

scarlett::RenderMeshGL::RenderMeshGL(aiMesh * mesh, const aiScene * world)
{
	auto mgrgl = (GraphicsMgrGL*)GApp->mGraphicsManager;
	Initialize(mesh);

	auto material = world->mMaterials[mesh->mMaterialIndex];
	aiString name;
	aiGetMaterialString(material, AI_MATKEY_NAME, &name);
	mMaterial = std::make_shared<MaterialGL>();
	aiColor4D diffuse;
	aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
	auto shader = mgrgl->GetShader("pbr");
	mMaterial->SetShader(shader);
	mMaterial->SetShaderParamter("color", Vector4f(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
}

scarlett::RenderMeshGL::RenderMeshGL(void* data, int count, VertexFormat vf)
{
	auto mgrgl = (GraphicsMgrGL*)GApp->mGraphicsManager;
	Initialize(data, count, vf);
	mMaterial = std::make_shared<MaterialGL>();
	auto shader = mgrgl->GetShader("debug");
	mMaterial->SetShader(shader);
}

scarlett::RenderMeshGL::~RenderMeshGL()
{
	Finialize();
}

void scarlett::RenderMeshGL::Initialize(aiMesh * mesh) noexcept
{
	if (!mesh) {
		return;
	}

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	auto count = mesh->mNumVertices;
	if (mesh->HasPositions()) {
		mPositions = std::make_shared<VertexBufferGL>(mesh->mVertices, count, VertexFormat::VF_P3F, 0);
	}

	if (mesh->HasNormals()) {
		mNormals = std::make_shared<VertexBufferGL>(mesh->mNormals, count, VertexFormat::VF_N3F, 1);
	}

	if (mesh->HasTextureCoords(0)) {
		float *texCoords = (float *)malloc(sizeof(float) * 2 * mesh->mNumVertices);
		for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
			texCoords[k * 2] = mesh->mTextureCoords[0][k].x;
			texCoords[k * 2 + 1] = mesh->mTextureCoords[0][k].y;
		}
		mTexCoords = std::make_shared<VertexBufferGL>(texCoords, count, VertexFormat::VF_T2F, 2);
		free(texCoords);
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
		mIndexes = std::make_shared<IndexBufferGL>(idata, count, IndexFormat::IF_UINT32);
		free(idata);
	}

	if (mesh->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_POINT) {
		mType = PrimitiveType::PT_POINT;
	}
	else if (mesh->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_LINE) {
		mType = PrimitiveType::PT_LINE;
	}
	else if (mesh->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_TRIANGLE){
		mType = PrimitiveType::PT_TRIANGLE;
	}
	else {
		SCARLETT_ASSERT(false);
	}
}

void scarlett::RenderMeshGL::Initialize(void* data, int count, VertexFormat vf) noexcept
{
	auto mgrgl = (GraphicsMgrGL*)GApp->mGraphicsManager;
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	mPositions = mgrgl->CreateVertexBuffer(data, count, vf);
	mType = PrimitiveType::PT_LINE;
}

void scarlett::RenderMeshGL::Render(World * world, const Matrix4f & worldMatrix) noexcept
{
	ConstantBuffer cb;
	auto camera = world->GetCameraSystem()->GetMainCamera()->GetComponent<CameraComponent>();
	cb.world = worldMatrix.transpose();
	cb.view = camera->GetViewMatrix().transpose();
	cb.projection = camera->GetPerspectiveMatrix().transpose();
	mMaterial->Apply(cb);

	glBindVertexArray(mVAO);
	if (mIndexes) {
		glDrawElements(GetMode(), mIndexes->mCount, GL_UNSIGNED_INT, 0x00);
	}
	else {
		glDrawArrays(GetMode(), 0x00, mPositions->mCount);
	}
}

void scarlett::RenderMeshGL::Finialize() noexcept
{
	mPositions = nullptr;
	mNormals = nullptr;
	mTexCoords = nullptr;
	mIndexes = nullptr;
	glDeleteVertexArrays(1, &mVAO);
}

GLenum scarlett::RenderMeshGL::GetMode()
{
	switch (mType)
	{
	case scarlett::PT_POINT:
		return GL_POINTS;
	case scarlett::PT_LINE:
		return GL_LINES;
	case scarlett::PT_TRIANGLE:
		return GL_TRIANGLES;
	default:
		SCARLETT_ASSERT(false);
		return 0;
	}
}

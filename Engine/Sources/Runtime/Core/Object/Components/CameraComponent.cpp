#include "Runtime/Core/Application/GlobalConfig.h"
#include "CameraComponent.h"
#include <DirectXMath.h>
#include "Runtime/Core/Application/Application.h"

using namespace DirectX;

scarlett::SkyBox::SkyBox(const std::string & path)
{
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	auto texture = GApp->mGraphicsManager->CreateTextureCubemap(path);
	auto sampler = GApp->mGraphicsManager->CreateSamplerState();
	mBox = GApp->mGraphicsManager->CreateRenderMeshDebug(skyboxVertices, 36, VF_P3F);
	mBox->mPrimitiveType = PrimitiveType::PT_TRIANGLE;
	auto shader = GApp->mGraphicsManager->GetShader("skybox");
	mBox->mMeshType = MeshType::MT_Skybox;
	mBox->mMaterial->SetName("skybox");
	mBox->mMaterial->SetShader(shader);
	mBox->mMaterial->SetTexture("skybox", texture);
	mBox->mMaterial->SetSamplerState("skybox", sampler);
	mBox->mMaterial->GetDepthStencilState()->SetFunc(DepthStencilStateFunc::LESS_EQUAL);

	mIrradianceMap = GApp->mGraphicsManager->CreateTextureCubemap("./Asset/Textures/skybox/output_iem.dds");
	mIrradianceMapSamplerState = GApp->mGraphicsManager->CreateSamplerState();

	mEnvmap = GApp->mGraphicsManager->CreateTextureCubemap("./Asset/Textures/skybox/output_pmrem.dds");
	mEnvmapSamplerState = GApp->mGraphicsManager->CreateSamplerState();
}

scarlett::SkyBox::~SkyBox()
{
}

void scarlett::SkyBox::Render()
{
	auto worldMatrix = Matrix4f::Identity();
	auto camera = GApp->mWorld->GetCameraSystem()->GetMainCamera()->GetComponent<CameraComponent>();
	auto view = camera->GetViewMatrixOrigin().transpose();
	auto projection = camera->GetPerspectiveMatrix().transpose();
	mBox->Render(worldMatrix, view, projection);
}


scarlett::CameraComponent::CameraComponent() :
	mCameraType(CameraType::Perspective),
	mPosition(Vector3f(10, 10, 20)),
	mLookat(Vector3f(0, 1, 0)),
	mUp(Vector3f(0, 1, 0)),
	mNearClip(0.01f),
	mFarClip(1000.0f),
	mFov(PI / 3),
	mViewDirty(true),
	mProjectionDirty(true),
	mSkybox(nullptr)
{
	SetSkybox("./Asset/Textures/skybox/output_skybox.dds");
}

int scarlett::CameraComponent::Initialize() noexcept
{
	return 0;
}

void scarlett::CameraComponent::Finalize() noexcept
{
}

const Matrix4f scarlett::CameraComponent::GetViewMatrix()
{
	if (mViewDirty) {
		mViewMatrix = BuildViewLookatLH(mPosition, mLookat, mUp);
		mViewDirty = false;
	}

	return mViewMatrix;
}

const Matrix4f scarlett::CameraComponent::GetViewMatrixOrigin()
{
	auto m = GetViewMatrix();
	m(0, 3) = 0.0f;
	m(1, 3) = 0.0f;
	m(2, 3) = 0.0f;
	return m;
}

const Matrix4f scarlett::CameraComponent::GetPerspectiveMatrix()
{	
	float width = GlobalConfig::GetInstance()->GetScreenWidth();
	float height = GlobalConfig::GetInstance()->GetScreenHeight();

	if (mProjectionDirty) {
		if (mCameraType == CameraType::Orth) {
			mProjectionMatrix = BuildOrthoLH(width, height, mNearClip, mFarClip);
		}
		else
		{
			mProjectionMatrix = BuildPerspectiveFovLH(mFov, width / height, mNearClip, mFarClip);
		}
	}
	return mProjectionMatrix;
}

void scarlett::CameraComponent::SetSkybox(const std::string & path)
{
	mSkybox = std::make_shared<SkyBox>(path);
}

std::shared_ptr<scarlett::SkyBox> scarlett::CameraComponent::GetSkybox()
{
	return mSkybox;
}



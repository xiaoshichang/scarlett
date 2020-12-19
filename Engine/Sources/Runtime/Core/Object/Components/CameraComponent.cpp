#include "Runtime/Core/Application/GlobalConfig.h"
#include "CameraComponent.h"
#include <DirectXMath.h>
#include "Runtime/Core/Application/Application.h"
#include "Runtime/Core/UI/Listener.h"

using namespace DirectX;
using namespace scarlett;

scarlett::SkyBox::SkyBox(const std::string & path, CameraComponent* comp):
	mComp(comp)
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
	auto entity = mComp->GetMaster();
	mBox->Render(entity);
}


scarlett::CameraComponent::CameraComponent() :
	mCameraType(CameraType::Perspective),
	mPosition(Vector3f(30, 30, 40)),
	mLookat(Vector3f(0, 0, 0)),
	mUp(Vector3f(0, 1, 0)),
	mNearClip(0.01f),
	mFarClip(2000.0f),
	mFov(PI / 3),
	mViewDirty(true),
	mProjectionDirty(true),
	mSkybox(nullptr),
	mSpeed(1.0f)
{
	//SetSkybox("./Asset/Textures/skybox/output_skybox.dds");


	auto listener = UIEventListenerKeyboard::create();
	listener->onKeyPressed = [=](unsigned char code, UIEvent* event) {
		if (code == 'W') {
			this->Forward();
		}
		else if (code == 'S') {
			this->Backward();
		}
	};

	GApp->mInputManager->mDispatcher->addEventListener(listener);
}

int scarlett::CameraComponent::Initialize() noexcept
{
	return 0;
}

void scarlett::CameraComponent::Finalize() noexcept
{
}

const Matrix4x4f scarlett::CameraComponent::GetViewMatrix()
{
	if (mViewDirty) {
		BuildMatrixViewLookatLH(mViewMatrix, mPosition, mLookat, mUp);
		mViewDirty = false;
	}

	return mViewMatrix;
}

const Matrix4x4f scarlett::CameraComponent::GetViewMatrixOrigin()
{
	auto m = GetViewMatrix();
	m[0][3] = 0.0f;
	m[1][3] = 0.0f;
	m[2][3] = 0.0f;
	return m;
}

const Matrix4x4f scarlett::CameraComponent::GetPerspectiveMatrix()
{	
	if (mProjectionDirty) {
		float width = GlobalConfig::GetInstance()->GetScreenWidth();
		float height = GlobalConfig::GetInstance()->GetScreenHeight();
		if (mCameraType == CameraType::Orth) {
			BuildMatrixOrthoLH(mProjectionMatrix, width, height, mNearClip, mFarClip);
		}
		else
		{
			BuildMatrixPerspectiveFovLH(mProjectionMatrix, mFov, width / height, mNearClip, mFarClip);
		}
		mProjectionDirty = false;
	}
	return mProjectionMatrix;
}

void scarlett::CameraComponent::SetSkybox(const std::string & path)
{
	mSkybox = std::make_shared<SkyBox>(path, this);
}

std::shared_ptr<scarlett::SkyBox> scarlett::CameraComponent::GetSkybox()
{
	return mSkybox;
}

void scarlett::CameraComponent::Forward()
{
	float dt = GApp->mTimeMgr->GetDeltaMsTime();
	Vector3f dir = mLookat - mPosition;
	Normalize(dir);
	auto delta = VectorScale(dir, dt * mSpeed);
	SetPosition(mPosition + delta);
}

void scarlett::CameraComponent::Backward()
{
	float dt = GApp->mTimeMgr->GetDeltaMsTime();
	Vector3f dir = mLookat - mPosition;
	Normalize(dir);
	auto delta = VectorScale(dir, dt * mSpeed);
	SetPosition(mPosition - delta);
}

void scarlett::CameraComponent::MoveLeft()
{
	float dt = GApp->mTimeMgr->GetDeltaMsTime();
	// todo
}

void scarlett::CameraComponent::MoveRight()
{
	float dt = GApp->mTimeMgr->GetDeltaMsTime();
	// todo
}



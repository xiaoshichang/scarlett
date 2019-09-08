#include "CameraComponent.h"
#include <DirectXMath.h>
using namespace DirectX;

scarlett::CameraComponent::CameraComponent() :
	mCameraType(CameraType::Perspective),
	mPosition(Vector3f(200, 200, 200)),
	mLookat(Vector3f(0, 0, 0)),
	mUp(Vector3f(0, 1, 0)),
	mNearClip(0.01f),
	mFarClip(1000.0f),
	mFov(PI / 3)
{
}

int scarlett::CameraComponent::Initialize() noexcept
{
	return 0;
}

void scarlett::CameraComponent::Finalize() noexcept
{
}

Matrix4f scarlett::CameraComponent::GetViewMatrix()
{
	return BuildViewLookatLH(mPosition, mLookat, mUp);
}

Matrix4f scarlett::CameraComponent::GetPerspectiveMatrix()
{	
	float width = 1024.0f;
	float height = 768.0f;
	if (mCameraType == CameraType::Orth) {
		return BuildOrthoLH(width, height, mNearClip, mFarClip);
	}
	else
	{
		return BuildPerspectiveFovLH(mFov, width / height, mNearClip, mFarClip);
	}

}

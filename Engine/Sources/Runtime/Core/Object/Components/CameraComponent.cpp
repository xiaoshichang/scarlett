#include "CameraComponent.h"

scarlett::CameraComponent::CameraComponent() :
	mCameraType(CameraType::Perspective),
	mPosition(Vector3f(0, 100, -100)),
	mLookat(Vector3f(0, 0, 0)),
	mUp(Vector3f(0, 1, 0)),
	mNearClip(1.0f),
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
	return BuildViewLookatRH(mPosition, mLookat, mUp);
}

Matrix4f scarlett::CameraComponent::GetPerspectiveMatrix()
{	
	float width = 1024.0f;
	float height = 768.0f;
	if (mCameraType == CameraType::Orth) {
		return BuildOrthoRH(width, height, mNearClip, mFarClip);
	}
	else
	{
		return BuildPerspectiveFovRH(mFov, width / height, mNearClip, mFarClip);
	}

}

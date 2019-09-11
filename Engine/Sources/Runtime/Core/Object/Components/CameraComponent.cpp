#include "CameraComponent.h"
#include <DirectXMath.h>
using namespace DirectX;

scarlett::CameraComponent::CameraComponent() :
	mCameraType(CameraType::Perspective),
	mPosition(Vector3f(-10, 10, -20)),
	mLookat(Vector3f(0, 1, 0)),
	mUp(Vector3f(0, 1, 0)),
	mNearClip(0.01f),
	mFarClip(1000.0f),
	mFov(PI / 3),
	mViewDirty(true),
	mProjectionDirty(true)
{
}

int scarlett::CameraComponent::Initialize() noexcept
{
	return 0;
}

void scarlett::CameraComponent::Finalize() noexcept
{
}

const Matrix4f& scarlett::CameraComponent::GetViewMatrix()
{
	if (mViewDirty) {
		mViewMatrix = BuildViewLookatLH(mPosition, mLookat, mUp);
		mViewDirty = false;
	}

	return mViewMatrix;
}

const Matrix4f& scarlett::CameraComponent::GetPerspectiveMatrix()
{	
	float width = 1024.0f;
	float height = 768.0f;

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

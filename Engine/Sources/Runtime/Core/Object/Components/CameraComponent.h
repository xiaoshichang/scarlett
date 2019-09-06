#pragma once
#include "Runtime/Core/Math/ScltMath.h"
#include "Runtime/Interface/IComponent.h"

namespace scarlett {

	enum CameraType
	{
		Orth = 1,
		Perspective = 2
	};

	class CameraComponent : public IComponent {
	public:
		CameraComponent();
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;

		Matrix4f	GetViewMatrix();
		Matrix4f	GetPerspectiveMatrix();
	public:
		Vector3f	mPosition;
		Vector3f	mLookat;
		Vector3f	mUp;

		CameraType	mCameraType;
		float		mNearClip;
		float		mFarClip;
		float		mFov;

	};

}
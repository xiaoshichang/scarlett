#pragma once
#include "Runtime/Core/Math/ScltMath.h"
#include "Runtime/Interface/IComponent.h"
#include "Runtime/RHI/Texture.h"
#include "Runtime/RHI/RenderMesh.h"

namespace scarlett {

	class SkyBox {
	public:
		SkyBox(const std::string& path);
		virtual ~SkyBox();
		void	Render();

	private:
		std::shared_ptr<RenderMesh> mBox;
	};


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

		const Matrix4f	GetViewMatrix();
		const Matrix4f  GetViewMatrixOrigin();
		const Matrix4f	GetPerspectiveMatrix();

		CameraType	GetType() { return mCameraType; }
		void		SetType(CameraType type) { mCameraType = type; mViewDirty = true; mProjectionDirty = true; }

		Vector3f	GetPosition() { return mPosition; }
		Vector3f	GetLookat() { return mLookat; }
		Vector3f	GetUp() { return mUp; }
		void		SetPosition(const Vector3f& position) { mPosition = position; mViewDirty = true; }
		void		SetLook(const Vector3f& lookat) { mLookat = lookat; mViewDirty = true; }
		void		SetUp(const Vector3f& up) { mUp = up; mViewDirty = true; }

		float		GetNearClip() { return mNearClip; }
		float		GetFarClip() { return mFarClip; }
		float		GetFov() { return mFov; }
		void		SetNearClip(float n) { mProjectionDirty = true; mNearClip = n; }
		void		SetFarClip(float f) { mProjectionDirty = true; mFarClip= f; }
		void		SetFov(float fov) { mFov = fov; mProjectionDirty = true; }

		void		SetSkybox(const std::string& path);
		std::shared_ptr<SkyBox>	GetSkybox();

	private:
		CameraType	mCameraType;

		bool		mViewDirty;
		Matrix4f	mViewMatrix;
		Vector3f	mPosition;
		Vector3f	mLookat;
		Vector3f	mUp;

		bool		mProjectionDirty;
		Matrix4f	mProjectionMatrix;
		float		mNearClip;
		float		mFarClip;
		float		mFov;

		std::shared_ptr<SkyBox>		mSkybox;
	};

}
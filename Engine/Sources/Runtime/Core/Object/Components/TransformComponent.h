#pragma once

#include "Runtime/Interface/IComponent.h"
#include "Runtime/Core/Math/ScltMath.h"

namespace scarlett {

	class TransformComponent : public IComponent {
	public:
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;

	public:
		TransformComponent();
		Vector3f	GetPosition() noexcept { return mPosition; }
		Vector3f	GetRotation() noexcept { return mRotation; }
		Vector3f	GetScale() noexcept { return mScale; }
		void		SetPosition(Vector3f position) { mPosition = position; mWorldMatrixDirty = true; mWorldMatrixInverseDirty = true; }
		void		SetRotation(Vector3f rotation) { mRotation = rotation; mWorldMatrixDirty = true; mWorldMatrixInverseDirty = true; }
		void		SetScale(Vector3f scale) { mScale = scale; mWorldMatrixDirty = true; mWorldMatrixInverseDirty = true; }
		Matrix4x4f	GetWorldMatrix();
		Matrix4x4f  GetWorldMatrixInverse();

		Vector3f	GlobalVectorToLocalVector(Vector3f globalVector);
		Vector3f	LocalPointToGlobalPoint(Vector3f localPoint);
		Matrix3x3f	GetRatationMatrixGlobal2Local33();

	private:
		bool		mWorldMatrixDirty;
		bool		mWorldMatrixInverseDirty;
		Matrix4x4f	mWorldMatrix;
		Matrix4x4f  mWorldMatrixInverse;
		Vector3f	mPosition;
		Vector3f	mRotation;
		Vector3f	mScale;

	};

}
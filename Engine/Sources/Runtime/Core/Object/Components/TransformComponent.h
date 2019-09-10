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
		void		SetPosition(Vector3f position) { mPosition = position; mWorldMatrixDirty = true; }
		void		SetRotation(Vector3f rotation) { mRotation = rotation; mWorldMatrixDirty = true; }
		void		SetScale(Vector3f scale) { mScale = scale; mWorldMatrixDirty = true;}
		Matrix4f	GetWorldMatrix();

	private:
		bool		mWorldMatrixDirty;
		Matrix4f	mWorldMatrix;
		Vector3f	mPosition;
		Vector3f	mRotation;
		Vector3f	mScale;

	};

}
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
		void		SetPosition(Vector3f position) { mPosition = position; }
		void		SetRotation(Vector3f rotation) { mRotation = rotation; }
		void		SetScale(Vector3f scale) { mScale = scale; }

	private:
		Vector3f mPosition;
		Vector3f mRotation;
		Vector3f mScale;

	};

}
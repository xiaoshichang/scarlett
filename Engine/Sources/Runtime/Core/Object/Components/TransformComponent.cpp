#include "Runtime/Core/Object/Components/TransformComponent.h"

using namespace scarlett;

TransformComponent::TransformComponent() :
	mPosition(0.0f, 0.0f, 0.0f),
	mRotation(0.0f, 0.0f, 0.0f),
	mScale(1.0f, 1.0f, 1.0f),
	mWorldMatrixDirty(true)
{}

Matrix4x4f scarlett::TransformComponent::GetWorldMatrix()
{
	if (mWorldMatrixDirty) {
		Matrix4x4f translation, scaling, rx, ry, rz;
		auto position = GetPosition();
		auto scale = GetScale();
		auto rotation = GetRotation();
		BuildMatrixTranslation(translation, position.x, position.y, position.z);
		BuildMatrixScale(scaling, scale.x, scale.y, scale.z);

		BuildMatrixRotationX(rx, rotation.x);
		BuildMatrixRotationY(ry, rotation.y);
		BuildMatrixRotationZ(rz, rotation.z);

		mWorldMatrix = translation * rz * ry * rx * scaling; // make sure translation matrix go first.
		mWorldMatrixDirty = false;
	}
	return mWorldMatrix;
}

int TransformComponent::Initialize() noexcept
{
	return 0;
}


void TransformComponent::Finalize() noexcept {
}

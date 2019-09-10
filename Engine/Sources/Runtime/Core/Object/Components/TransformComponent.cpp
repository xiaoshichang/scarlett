#include "Runtime/Core/Object/Components/TransformComponent.h"

using namespace scarlett;

TransformComponent::TransformComponent() :
	mPosition(0.0f, 0.0f, 0.0f),
	mRotation(0.0f, 0.0f, 0.0f),
	mScale(1.0f, 1.0f, 1.0f),
	mWorldMatrixDirty(true)
{}

Matrix4f scarlett::TransformComponent::GetWorldMatrix()
{
	if (mWorldMatrixDirty) {
		auto position = GetPosition();
		auto scale = GetScale();
		auto rotation = GetRotation();
		auto translation = BuildTranslationMatrix(position);
		auto scaling = BuildScaleMatrix(scale);

		auto rx = BuildRotationMatrix(Vector3f(1, 0, 0), rotation.x());
		auto ry = BuildRotationMatrix(Vector3f(0, 1, 0), rotation.y());
		auto rz = BuildRotationMatrix(Vector3f(0, 0, 1), rotation.z());
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

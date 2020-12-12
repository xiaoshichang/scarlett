#include "Runtime/Core/Object/Components/TransformComponent.h"

using namespace scarlett;

TransformComponent::TransformComponent() :
	mPosition(0.0f, 0.0f, 0.0f),
	mRotation(0.0f, 0.0f, 0.0f),
	mScale(1.0f, 1.0f, 1.0f),
	mWorldMatrixDirty(true),
	mWorldMatrixInverseDirty(true)
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

Matrix4x4f scarlett::TransformComponent::GetWorldMatrixInverse()
{
	if (mWorldMatrixInverseDirty)
	{
		Matrix4x4f translation, scaling, rx, ry, rz;
		auto position = GetPosition();
		auto scale = GetScale();
		auto rotation = GetRotation();
		BuildMatrixTranslation(translation, -position.x, -position.y, -position.z);
		BuildMatrixScale(scaling, 1 / scale.x, 1 / scale.y, 1 / scale.z);

		BuildMatrixRotationX(rx, -rotation.x);
		BuildMatrixRotationY(ry, -rotation.y);
		BuildMatrixRotationZ(rz, -rotation.z);
		mWorldMatrixInverse = scaling * rx * ry * rz * translation ; // make sure translation matrix go first.
	}
	
	return mWorldMatrixInverse;
}

Vector3f scarlett::TransformComponent::GlobalVectorToLocalVector(Vector3f globalVector)
{
	Matrix4x4f world2local = GetWorldMatrixInverse();
	Vector3f localVector = TransformVector(world2local, globalVector);
	return localVector;
}

Vector3f scarlett::TransformComponent::LocalPointToGlobalPoint(Vector3f localPoint)
{
	Matrix4x4f local2world = GetWorldMatrix();
	Vector3f globalPoint = TransformPoint(local2world, localPoint);
	return globalPoint;
}

int TransformComponent::Initialize() noexcept
{
	return 0;
}


void TransformComponent::Finalize() noexcept {
}

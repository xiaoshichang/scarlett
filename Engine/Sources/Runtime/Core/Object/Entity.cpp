#include <type_traits>
#include <iostream>
#include "Runtime/Core/Object/Entity.h"

int scarlett::Entity::Initialize() noexcept {
	return 0;
}

int scarlett::Entity::Initialize(scarlett::World* world) noexcept {
	mWorld = world;

	AddComponent<TransformComponent>();

	return 0;
}

void scarlett::Entity::Finalize() noexcept {
	mChildren.clear();
	mParent = nullptr;

	if (mTransform) {
		RemoveComponent<TransformComponent>();
	}
	if (mMeshRender) {
		RemoveComponent<MeshRenderComponent>();
	}
	if (mCamera) {
		RemoveComponent<CameraComponent>();
	}
	if (mSkeleton) {
		RemoveComponent<SkeletonComponent>();
	}
}

scarlett::Entity::Entity():
	mParent(nullptr),
	mTransform(nullptr),
	mMeshRender(nullptr),
	mCamera(nullptr),
	mSkeleton(nullptr)
{
	mGuid = boost::uuids::random_generator()();
}

scarlett::Entity::Entity(const boost::uuids::uuid& guid):
	mParent(nullptr),
	mTransform(nullptr),
	mMeshRender(nullptr),
	mCamera(nullptr),
	mSkeleton(nullptr),
	mGuid(guid)
{
}

scarlett::Entity::~Entity() {
	std::cout << "destructor of entity: " << mGuid << std::endl;
	Finalize();
}

boost::uuids::uuid scarlett::Entity::GetGuid() const noexcept
{
	return mGuid;
}

void scarlett::Entity::SetGuid(const boost::uuids::uuid & guid) noexcept
{
	mGuid = guid;
}

void scarlett::Entity::AddChild(std::shared_ptr<Entity> child) {
	if (IsChild(child)) {
		return;
	}
	mChildren.push_back(child);
	child->SetParent(this);
}

void scarlett::Entity::RemoveChild(std::shared_ptr<Entity> child) {
	if (!IsChild(child)) {
		return;
	}
	auto iter = mChildren.begin();
	for (; iter != mChildren.end(); ++iter) {
		if ((*iter) == child) {
			mChildren.erase(iter);
			child->SetParent(nullptr);
			return;
		}
	}
}

bool scarlett::Entity::IsChild(std::shared_ptr<Entity> child) {
	if (child->GetParent() == this) {
		return true;
	}
	return false;
}

scarlett::Entity* scarlett::Entity::GetParent() {
	return mParent;
}

void	scarlett::Entity::SetParent(scarlett::Entity* parent) {
	mParent = parent;
}

size_t  scarlett::Entity::GetChildrenCount() {
	return mChildren.size();
}
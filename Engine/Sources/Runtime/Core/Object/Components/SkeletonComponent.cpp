#include "SkeletonComponent.h"
#include "Foundation/Assert.h"
#include "Runtime/Core/Object/Entity.h"
#include "Runtime/Core/Object/World.h"
#include "Runtime/Utils/Convert.h"

scarlett::Skeleton::Skeleton(const aiNode* armature, const aiScene* scene)
{
	mRoot = armature;
	mScene = scene;

	auto node = scene->mRootNode->mChildren[1];

	for (int i = 0; i < scene->mNumMeshes; i++) {
		auto mesh = scene->mMeshes[i];
		for (int j = 0; j < mesh->mNumBones; j++) {
			auto bone = mesh->mBones[j];
			std::string name(bone->mName.C_Str());

			if (!mBoneMap[name]) {
				mBoneMap[name] = j;
				AssimpMatrix2Eigen(bone->mOffsetMatrix, mBoneOffeset[j]);
			}
		}
	}
}

scarlett::Skeleton::~Skeleton()
{
}

void scarlett::Skeleton::CalculateFinalMatrix()
{
	_CalculateFinalMatrix(mRoot, Matrix4f::Identity());
}

void scarlett::Skeleton::_CalculateFinalMatrix(const aiNode * node, const Matrix4f & parentTransform)
{
	auto nodeName = node->mName;
	std::string name(nodeName.C_Str());
	auto transform = node->mTransformation;
	Matrix4f NodeTransformation;
	AssimpMatrix2Eigen(transform, NodeTransformation);

	Matrix4f globalMatrix = NodeTransformation * parentTransform;

	if (mBoneMap.find(name) != mBoneMap.end()) {
		int boneIndex = mBoneMap[name];
		mBoneTransforms[boneIndex] = mBoneOffeset[boneIndex] * globalMatrix;
		mBoneTransforms[boneIndex].transposeInPlace();
	}

	for (int i = 0; i < node->mNumChildren; i++) {
		_CalculateFinalMatrix(node->mChildren[i], globalMatrix);
	}
}


scarlett::SkeletonComponent::SkeletonComponent()
	: mSkeleton(nullptr)
{
}

scarlett::SkeletonComponent::~SkeletonComponent()
{
	if (mSkeleton) {
		delete mSkeleton;
	}
}

int scarlett::SkeletonComponent::Initialize() noexcept
{
	auto master = GetMaster();
	if (master) {
		auto world = master->GetWorld();
		if (world) {
			world->GetAnimationSystem()->AddComponent(this);
		}
	}
	return 0;
}

void scarlett::SkeletonComponent::Finalize() noexcept
{
	auto master = GetMaster();
	if (master) {
		auto world = master->GetWorld();
		if (world) {
			world->GetAnimationSystem()->DeleteComponent(this);
		}
	}
}

void scarlett::SkeletonComponent::Tick() noexcept
{
	mSkeleton->CalculateFinalMatrix();
}

void scarlett::SkeletonComponent::InitializeHeirarchy(const aiNode * armature, const aiScene* scene) noexcept
{
	mSkeleton = new Skeleton(armature, scene);
}

void scarlett::SkeletonComponent::UpdateBoneTransforms()
{
}



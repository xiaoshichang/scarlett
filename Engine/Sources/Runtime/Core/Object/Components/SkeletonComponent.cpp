#include "SkeletonComponent.h"


scarlett::Joint::Joint()
{
}

scarlett::Skeleton::Skeleton()
{
}

void scarlett::Skeleton::InitFromArmatureNode(aiNode * node, int parent)
{
	auto name = std::string(node->mName.C_Str());
	auto index = mJoints.size();
	mIndex[name] = index;

	auto joint = new Joint();
	joint->parent = parent;
	mJoints.push_back(joint);

	for (int i = 0; i < node->mNumChildren; ++i) {
		auto child = node->mChildren[i];
		InitFromArmatureNode(child, index);
	}

}

scarlett::Joint * scarlett::Skeleton::GetJoint(std::string name)
{
	auto index = mIndex[name];
	return mJoints[index];
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
	return 0;
}

void scarlett::SkeletonComponent::Finalize() noexcept
{
}

void scarlett::SkeletonComponent::SetDataFromScene(const aiScene * scene) noexcept
{
	auto armature = scene->mRootNode->FindNode("Armature");
	if (!armature) {
		return;
	}
	mSkeleton = new Skeleton();
	mSkeleton->InitFromArmatureNode(armature, 0xFF);

}

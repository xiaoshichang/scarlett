#include "SkeletonComponent.h"
#include "Foundation/Assert.h"
#include "Runtime/Core/Object/Entity.h"
#include "Runtime/Core/Object/World.h"
#include "Runtime/Utils/Convert.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/Utils/Logging.h"

using namespace scarlett;

scarlett::Skeleton::Skeleton(const aiNode* armature, const aiScene* scene)
{
	mRoot = armature;
	mScene = scene;
	
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

	auto count = scene->mNumAnimations;
	auto a = scene->mAnimations[0];
}

scarlett::Skeleton::~Skeleton()
{
}

void scarlett::Skeleton::CalculateFinalMatrix()
{
	auto m1 = mRoot->mParent->mTransformation;
	auto m2 = mRoot->mParent->mParent->mTransformation;
	auto m3 = mRoot->mParent->mParent->mParent->mTransformation;
	auto m4 = mRoot->mParent->mParent->mParent->mParent->mTransformation;
	auto m = m4*m3*m2*m1;
	Matrix4x4f _m;
	AssimpMatrix2Eigen(m, _m);
	_CalculateFinalMatrix(mRoot, _m);
}

void scarlett::Skeleton::_CalculateFinalMatrix(const aiNode * node, const Matrix4x4f & parentTransform)
{
	
	auto nodeName = node->mName;
	std::string name(nodeName.C_Str());
	auto transform = node->mTransformation;
	Matrix4x4f NodeTransformation;

	float TicksPerSecond = mScene->mAnimations[0]->mTicksPerSecond != 0 ?
		mScene->mAnimations[0]->mTicksPerSecond : 25.0f;

	float animationTime = GApp->mTimeMgr->GetTotalMsTime();
	float TimeInTicks = animationTime / 1000 * TicksPerSecond;
	animationTime = fmod(TimeInTicks, mScene->mAnimations[0]->mDuration);

	const aiAnimation* pAnimation = mScene->mAnimations[0];
	
	aiNodeAnim* anim = nullptr;
	for (int i = 0; i < pAnimation->mNumChannels; i++) {
		if (pAnimation->mChannels[i]->mNodeName == node->mName) {
			anim = pAnimation->mChannels[i];
		}
	}
	if (anim) {
		aiMatrix4x4 S, T;
		aiVector3D Scaling;
		CalcInterpolatedScale(Scaling, animationTime, anim);
		aiMatrix4x4::Scaling(Scaling, S);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, animationTime, anim);
		auto r = RotationQ.GetMatrix();
		aiMatrix4x4 R(r);

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedTranslation(Translation, animationTime, anim);
		aiMatrix4x4::Translation(Translation, T);


		transform = T * R * S;
	}

	AssimpMatrix2Eigen(transform, NodeTransformation);

	Matrix4x4f globalMatrix =  parentTransform * NodeTransformation;

	if (mBoneMap.find(name) != mBoneMap.end()) {
		int boneIndex = mBoneMap[name];
		mBoneTransforms[boneIndex] =  globalMatrix * mBoneOffeset[boneIndex];
	}

	for (int i = 0; i < node->mNumChildren; i++) {
		_CalculateFinalMatrix(node->mChildren[i], globalMatrix);
	}
}

void scarlett::Skeleton::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	int NextRotationIndex = (RotationIndex + 1);
	SCARLETT_ASSERT(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime;
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	SCARLETT_ASSERT(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}

void scarlett::Skeleton::CalcInterpolatedScale(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	int idx = FindScale(AnimationTime, pNodeAnim);
	int idxp1 = (idx + 1);
	SCARLETT_ASSERT(idxp1 < pNodeAnim->mNumScalingKeys);
	float DeltaTime = pNodeAnim->mScalingKeys[idxp1].mTime - pNodeAnim->mScalingKeys[idx].mTime;
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[idx].mTime) / DeltaTime;
	SCARLETT_ASSERT(Factor >= 0.0f && Factor <= 1.0f);

	const aiVector3D& StartScale = pNodeAnim->mScalingKeys[idx].mValue;
	const aiVector3D& EndScale = pNodeAnim->mScalingKeys[idxp1].mValue;

	float x = StartScale.x + (EndScale.x - StartScale.x)* Factor;
	float y = StartScale.y + (EndScale.y - StartScale.y) * Factor;
	float z = StartScale.z + (EndScale.z - StartScale.z) * Factor;
	Out.Set(x, y, z);
}

void scarlett::Skeleton::CalcInterpolatedTranslation(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	int idx = FindTranslation(AnimationTime, pNodeAnim);
	int idxp1 = (idx + 1);
	SCARLETT_ASSERT(idxp1 < pNodeAnim->mNumPositionKeys);
	float DeltaTime = pNodeAnim->mPositionKeys[idxp1].mTime - pNodeAnim->mPositionKeys[idx].mTime;
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[idx].mTime) / DeltaTime;
	SCARLETT_ASSERT(Factor >= 0.0f && Factor <= 1.0f);

	const aiVector3D& StartPos = pNodeAnim->mPositionKeys[idx].mValue;
	const aiVector3D& EndPos = pNodeAnim->mPositionKeys[idxp1].mValue;

	float x = StartPos.x + (EndPos.x - StartPos.x) * Factor;
	float y = StartPos.y + (EndPos.y - StartPos.y) * Factor;
	float z = StartPos.z + (EndPos.z - StartPos.z) * Factor;
	Out.Set(x, y, z);
}

int scarlett::Skeleton::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	SCARLETT_ASSERT(pNodeAnim->mNumRotationKeys > 0);

	for (int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	SCARLETT_ASSERT(false);
}

int scarlett::Skeleton::FindScale(float AnimationTime, const aiNodeAnim* pNodeAnim)
{

	SCARLETT_ASSERT(pNodeAnim->mNumScalingKeys > 0);

	for (int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}

	SCARLETT_ASSERT(false);
}

int scarlett::Skeleton::FindTranslation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	SCARLETT_ASSERT(pNodeAnim->mNumPositionKeys > 0);

	for (int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}

	SCARLETT_ASSERT(false);
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



#pragma once
#include <unordered_map>
#include <vector>
#include "Runtime/Interface/IComponent.h"
#include "Runtime/Core/Math/ScltMath.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"


namespace scarlett {

	class Skeleton {
	public:
		Skeleton(const aiNode* armature, const aiScene* scene);
		virtual ~Skeleton();
		void	CalculateFinalMatrix();
		void	_CalculateFinalMatrix(const aiNode* node, const Matrix4x4f& parentTransform);

		void	CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		void	CalcInterpolatedScale(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		void	CalcInterpolatedTranslation(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		int		FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
		int		FindScale(float AnimationTime, const aiNodeAnim* pNodeAnim);
		int		FindTranslation(float AnimationTime, const aiNodeAnim* pNodeAnim);


	public:
		const aiNode*							mRoot;
		const aiScene*							mScene;
		std::unordered_map<std::string, int>	mBoneMap;
		Matrix4x4f								mOrigin;
		Matrix4x4f								mBoneTransforms[32];
		Matrix4x4f								mBoneOffeset[32];
	};

	class SkeletonComponent : public IComponent{
	public:
		SkeletonComponent();
		virtual ~SkeletonComponent();
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;
		virtual void Tick() noexcept;
		virtual void InitializeHeirarchy(const aiNode* armature, const aiScene* scene) noexcept;
		virtual void UpdateBoneTransforms();


	public:
		Skeleton* mSkeleton;
	};
}
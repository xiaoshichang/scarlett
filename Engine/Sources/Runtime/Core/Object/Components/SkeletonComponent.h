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
		void	_CalculateFinalMatrix(const aiNode* node, const Matrix4f& parentTransform);

	public:
		const aiNode*							mRoot;
		const aiScene*							mScene;
		std::unordered_map<std::string, int>	mBoneMap;
		Matrix4f								mBoneTransforms[32];
		Matrix4f								mBoneOffeset[32];
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
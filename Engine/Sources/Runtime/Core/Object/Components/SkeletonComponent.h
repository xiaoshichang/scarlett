#pragma once
#include <unordered_map>
#include <vector>
#include "Runtime/Interface/IComponent.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

namespace scarlett {

	class Joint {
	public:
		Joint();

	public:
		unsigned char parent;
	};

	class Skeleton {
	public:
		Skeleton();
		void	InitFromArmatureNode(aiNode* node, int parent);
		Joint*	GetJoint(std::string name);

	public:
		std::vector<Joint*> mJoints;
		std::unordered_map<std::string, int> mIndex;
	};

	class SkeletonComponent : public IComponent{
	public:
		SkeletonComponent();
		virtual ~SkeletonComponent();
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;
		virtual void SetDataFromScene(const aiScene* scene) noexcept;

	public:
		Skeleton*	mSkeleton;

	};
}
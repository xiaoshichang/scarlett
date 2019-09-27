#pragma once
#include "Runtime/Interface/IComponent.h"

namespace scarlett {

	class Bone {
	public:
		Bone();
	};

	class Skeleton {
	public:
		Skeleton();
	};

	class SkeletonComponent : public IComponent{
	public:
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;


	};
}
#pragma once
#include "Runtime/Interface/IResource.h"

namespace scarlett {
	class GraphicsManager;

	class IndexBuffer : public IIndexBuffer {
	public:
		virtual void Initialize(GraphicsManager* mgr, void* data, unsigned int count, IndexFormat iformat) noexcept;

	public:
		unsigned int	mCount;
		IndexFormat		mIndexFormat;
	};

}
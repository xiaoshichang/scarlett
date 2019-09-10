#pragma once
#include "Runtime/Interface/IResource.h"

namespace scarlett {

	class IndexBuffer : public IIndexBuffer {
	public:
		virtual void Initialize(void* data, unsigned int count, IndexFormat iformat) noexcept;
		virtual void Finialize() noexcept;

	public:
		unsigned int	mCount;
		IndexFormat		mIndexFormat;
	};

}
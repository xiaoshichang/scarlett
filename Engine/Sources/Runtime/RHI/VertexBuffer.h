#pragma once

#include "Runtime/Interface/IResource.h"

namespace scarlett {
	class VertexBuffer : public IVertexBuffer{
	public:
		virtual void Initialize(void* data, unsigned int count, VertexFormat vf) noexcept;
		virtual void Finialize() noexcept;
		virtual unsigned int GetVertexSize(VertexFormat vf) noexcept;

	public:
		unsigned int mCount;
		VertexFormat mVertexFormat;

	};

}
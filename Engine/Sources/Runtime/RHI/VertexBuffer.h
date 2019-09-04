#pragma once

#include "Runtime/Interface/IResource.h"

namespace scarlett {

	class VertexBuffer : public IVertexBuffer{
	public:
		VertexBuffer(unsigned int count, VertexFormat vf) : mCount(count) { mVertexFormat = VertexFormat::None; }
		virtual		size_t	GetVertexSize(VertexFormat vf) noexcept;
	public:
		unsigned int mCount;
		VertexFormat mVertexFormat;

	};

}
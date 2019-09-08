#pragma once

#include "Runtime/Interface/IResource.h"

namespace scarlett {
	class GraphicsManager;
	class VertexBuffer : public IVertexBuffer{
	public:
		virtual void Initialize(GraphicsManager* mgr, void* data, unsigned int count, VertexFormat vf) noexcept;
		virtual unsigned int GetVertexSize(VertexFormat vf) noexcept;
	public:
		unsigned int mCount;
		VertexFormat mVertexFormat;

	};

}
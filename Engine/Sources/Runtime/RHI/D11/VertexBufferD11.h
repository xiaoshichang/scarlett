#pragma once

#include "Runtime/RHI/VertexBuffer.h"
#include <d3d11.h>

namespace scarlett {
	class GraphicsManager;
	class VertexBufferD11 : public VertexBuffer {
	public:
		virtual void Initialize(GraphicsManager* mgr, void* data, unsigned int count, VertexFormat vf) noexcept;

	public:
		ID3D11Buffer *mVertexBuffer;
	};
}
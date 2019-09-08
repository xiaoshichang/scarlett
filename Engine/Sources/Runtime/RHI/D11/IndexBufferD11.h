#pragma once
#include "Runtime/RHI/IndexBuffer.h"
#include <d3d11.h>

namespace scarlett {
	class GraphicsManager;

	class IndexBufferD11 : public IndexBuffer {
	public:

		virtual void Initialize(GraphicsManager* mgr, void* data, unsigned int count, IndexFormat iformat) noexcept;

	public:
		ID3D11Buffer*	mIndexBuffer;
	};

}
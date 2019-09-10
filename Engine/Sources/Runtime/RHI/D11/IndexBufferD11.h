#pragma once
#include "Runtime/RHI/IndexBuffer.h"
#include <d3d11.h>

namespace scarlett {
	class GraphicsManager;

	class IndexBufferD11 : public IndexBuffer {
	public:
		IndexBufferD11(void* data, unsigned int count, IndexFormat iformat);
		virtual ~IndexBufferD11();
		virtual void Initialize(void* data, unsigned int count, IndexFormat iformat) noexcept;
		virtual void Finialize() noexcept;

	public:
		ID3D11Buffer*	mIndexBuffer;
	};

}
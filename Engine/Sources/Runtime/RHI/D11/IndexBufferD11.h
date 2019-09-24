#pragma once
#include "Runtime/RHI/IndexBuffer.h"
#include <d3d11.h>

namespace scarlett {

	class IndexBufferD11 : public IIndexBuffer {
	public:
		virtual void Initialize(void* data, unsigned int count, IndexFormat iformat) noexcept;

		IndexBufferD11(void* data, unsigned int count, IndexFormat iformat);
		virtual ~IndexBufferD11();


	public:
		ID3D11Buffer*	mIndexBuffer;
	};

}
#pragma once

#include "Runtime/RHI/VertexBuffer.h"
#include <d3d11.h>

namespace scarlett {
	class VertexBufferD11 : public IVertexBuffer {
	public:
		VertexBufferD11(void* data, unsigned int count, VertexFormat vf);
		virtual ~VertexBufferD11();

		virtual void Initialize(void* data, unsigned int count, VertexFormat vf) noexcept;

	public:
		ID3D11Buffer *mVertexBuffer;
	};
}
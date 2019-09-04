#pragma once

#include "Runtime/RHI/VertexBuffer.h"

#include <d3d11.h>

namespace scarlett {

	class VertexBufferD11 : public VertexBuffer {
	public:
		VertexBufferD11(unsigned int count, VertexFormat vf);

	public:
		ID3D11Buffer *mVertexBuffer;
	};
}
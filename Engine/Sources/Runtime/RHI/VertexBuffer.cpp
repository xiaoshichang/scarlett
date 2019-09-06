#include "VertexBuffer.h"
#include "Runtime/RHI/GraphicsMgr.h"

void scarlett::VertexBuffer::Initialize(GraphicsManager * mgr, void * data, unsigned int count, VertexFormat vf) noexcept
{
}

size_t scarlett::VertexBuffer::GetVertexSize(VertexFormat vf) noexcept
{
	if (vf == VertexFormat::VF_P3F || vf == VertexFormat::VF_N3F) {
		return sizeof(float) * 3;
	}
	else if (vf == VertexFormat::VF_C4B) {
		return 4;
	}
	else if (vf == VertexFormat::VF_T2F) {
		return sizeof(float) * 2;
	}
	else {
		return 0;
	}
}

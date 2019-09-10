#include "VertexBuffer.h"
#include "Runtime/RHI/GraphicsMgr.h"

void scarlett::VertexBuffer::Initialize(void * data, unsigned int count, VertexFormat vf) noexcept
{
	mCount = count;
	mVertexFormat = vf;
}

unsigned int scarlett::VertexBuffer::GetVertexSize(VertexFormat vf) noexcept
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

void scarlett::VertexBuffer::Finialize() noexcept
{
}


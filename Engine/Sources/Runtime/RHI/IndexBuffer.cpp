#include "IndexBuffer.h"

void scarlett::IndexBuffer::Initialize(GraphicsManager * mgr, void * data, unsigned int count, IndexFormat iformat) noexcept
{
	mCount = count;
	mIndexFormat = iformat;
}

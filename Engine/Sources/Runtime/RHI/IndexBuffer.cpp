#include "IndexBuffer.h"


void scarlett::IndexBuffer::Initialize(void * data, unsigned int count, IndexFormat iformat) noexcept
{
	mCount = count;
	mIndexFormat = iformat;
}

void scarlett::IndexBuffer::Finialize() noexcept
{
}

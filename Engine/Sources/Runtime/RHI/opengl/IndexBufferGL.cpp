#include "Runtime/RHI/opengl/IndexBufferGL.h"



scarlett::IndexBufferGL::IndexBufferGL(void * data, unsigned int count, IndexFormat iformat)
{
	mIndexCount = count;
	mIndexFormat = iformat;
	Initialize(data, count, iformat);
}

scarlett::IndexBufferGL::~IndexBufferGL()
{
	glDeleteBuffers(1, &mVEO);
}

void scarlett::IndexBufferGL::Initialize(void * data, unsigned int count, IndexFormat iformat) noexcept
{
	glGenBuffers(1, &mVEO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVEO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * 4, data, GL_STATIC_DRAW);	// count * 4 == datasize

}

#include "Runtime/RHI/opengl/VertexBufferGL.h"
#include "Foundation/Assert.h"


scarlett::VertexBufferGL::VertexBufferGL(void * data, unsigned int count, VertexFormat vf, int index): mIndex(index)
{
	Initialize(data, count, vf);
}

scarlett::VertexBufferGL::~VertexBufferGL()
{
	glDeleteBuffers(1, &mVBO);
}

void scarlett::VertexBufferGL::Initialize(void * data, unsigned int count, VertexFormat vf) noexcept
{
	mVertexCount = count;
	mVertexFormat = vf;
	size_t dataSize = GetVertexSize(vf) * count;
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(mIndex);

	switch (vf)
	{
	case scarlett::VF_None:
		SCARLETT_ASSERT(false);
	case scarlett::VF_P3F:
		glVertexAttribPointer(mIndex, 3, GL_FLOAT, false, 0, 0);
		break;
	case scarlett::VF_T2F:
		glVertexAttribPointer(mIndex, 2, GL_FLOAT, false, 0, 0);
		break;
	case scarlett::VF_N3F:
		glVertexAttribPointer(mIndex, 3, GL_FLOAT, false, 0, 0);
		break;
	case scarlett::VF_C4B:
		glVertexAttribPointer(mIndex, 4, GL_FLOAT, false, 0, 0);
		break;
	default:
		SCARLETT_ASSERT(false);
	}
}

#include "Runtime/RHI/VertexBuffer.h"
#include "glad/glad_wgl.h"


namespace scarlett {

	class VertexBufferGL : public IVertexBuffer {
	public:
		virtual void Initialize(void* data, unsigned int count, VertexFormat vf) noexcept;

	public:
		VertexBufferGL(void* data, unsigned int count, VertexFormat vf, int index);
		virtual ~VertexBufferGL();

	public:
		int mIndex;
		GLuint mVBO;

	};
}
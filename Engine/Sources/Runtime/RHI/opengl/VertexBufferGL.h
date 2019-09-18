#include "Runtime/RHI/VertexBuffer.h"
#include "glad/glad_wgl.h"


namespace scarlett {

	class VertexBufferGL : public VertexBuffer {
	public:
		VertexBufferGL(void* data, unsigned int count, VertexFormat vf, int index);
		virtual ~VertexBufferGL();
		virtual void Initialize(void* data, unsigned int count, VertexFormat vf) noexcept;
		virtual void Finialize() noexcept;
	public:
		int mIndex;
		GLuint mVBO;

	};
}
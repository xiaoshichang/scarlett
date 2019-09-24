#include "Runtime/RHI/IndexBuffer.h"
#include "glad/glad_wgl.h"


namespace scarlett {

	class IndexBufferGL : public IIndexBuffer {
	public:
		IndexBufferGL(void* data, unsigned int count, IndexFormat iformat);
		virtual ~IndexBufferGL();
		virtual void Initialize(void* data, unsigned int count, IndexFormat iformat) noexcept;
	public:
		GLuint mVEO;

	};
}
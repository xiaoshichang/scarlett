#include "Runtime/RHI/IndexBuffer.h"
#include "glad/glad_wgl.h"


namespace scarlett {

	class IndexBufferGL : public IndexBuffer {
	public:
		IndexBufferGL(void* data, unsigned int count, IndexFormat iformat);
		virtual ~IndexBufferGL();
		virtual void Initialize(void* data, unsigned int count, IndexFormat iformat) noexcept;
		virtual void Finialize() noexcept;
	public:
		GLuint mVEO;

	};
}
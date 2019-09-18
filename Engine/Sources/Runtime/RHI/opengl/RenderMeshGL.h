#include "Runtime/RHI/RenderMesh.h"
#include "glad/glad_wgl.h"

namespace scarlett {

	class RenderMeshGL : public RenderMesh {

	public:
		RenderMeshGL(aiMesh* mesh, const aiScene* world);
		RenderMeshGL(void* data, int count, VertexFormat vf);
		virtual ~RenderMeshGL();
		virtual void		Initialize(aiMesh* mesh) noexcept;
		virtual void		Initialize(void* data, int count, VertexFormat vf) noexcept;
		virtual void		Render(World* world, const Matrix4f& worldMatrix) noexcept;
		virtual void		Finialize() noexcept;

	private:
		GLenum GetMode();

		GLuint mVAO;

	};

}
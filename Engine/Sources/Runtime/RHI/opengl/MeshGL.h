#include "Runtime/RHI/Mesh.h"
#include "glad/glad_wgl.h"

namespace scarlett {

	class MeshGL : public IMesh {

	public:
		MeshGL(aiMesh* mesh, const aiScene* world);
		MeshGL(void* data, int count, VertexFormat vf);
		virtual ~MeshGL();
		virtual void		Initialize(aiMesh* mesh) noexcept;
		virtual void		Initialize(void* data, int count, VertexFormat vf) noexcept;
		virtual void		Render(World* world, const Matrix4f& worldMatrix) noexcept;

	private:
		GLenum GetMode();

		GLuint mVAO;

	};

}
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
		virtual void		InitializeUI() noexcept;
		virtual void		Render(const Matrix4f& worldMatrix, const Matrix4f& viewMatrix, const Matrix4f& projectMatrix) noexcept;
		virtual void		RenderWithSkin(const Matrix4f& worldMatrix, const Matrix4f& viewMatrix, const Matrix4f& projectMatrix, const Matrix4f boneMatrix[], const int boneCount) noexcept;
	private:
		GLenum GetMode();

		GLuint mVAO;

	};

}
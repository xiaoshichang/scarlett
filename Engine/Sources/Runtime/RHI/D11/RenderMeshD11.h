#pragma once
#include "Runtime/RHI/RenderMesh.h"

#include <d3d11.h>

namespace scarlett {
	class RenderMeshD11 : public RenderMesh {
	public:
		RenderMeshD11(aiMesh* mesh, const aiScene* world);
		RenderMeshD11(std::shared_ptr<VertexBuffer> vb);
		virtual ~RenderMeshD11();
		virtual void		Initialize(aiMesh* mesh) noexcept;
		virtual void		Initialize(std::shared_ptr<VertexBuffer> vb) noexcept;
		virtual void		Render(World* world, const Matrix4f& worldMatrix) noexcept;
		virtual void		Finialize() noexcept;

	private:
		unsigned int*		stride;
		unsigned int*		offset;
		ID3D11Buffer **		vbuffers;
		unsigned int		vbcount;
	};

}
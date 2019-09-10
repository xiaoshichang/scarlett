#pragma once
#include "Runtime/RHI/RenderMesh.h"
#include <d3d11.h>

namespace scarlett {
	class RenderMeshD11 : public RenderMesh {
	public:
		virtual void		Initialize(GraphicsManager* mgr, aiMesh* mesh) noexcept;
		virtual void		Initialize(GraphicsManager* mgr, std::shared_ptr<VertexBuffer> vb) noexcept;
		virtual void		Render(GraphicsManager* mgr, World* world, const Matrix4f& worldMatrix) noexcept;

	private:
		unsigned int*		stride;
		unsigned int*		offset;
		ID3D11Buffer **		vbuffers;
		unsigned int		vbcount;
	};

}
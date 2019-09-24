#pragma once
#include "Runtime/RHI/Mesh.h"
#include <d3d11.h>

namespace scarlett {
	class MeshD11 : public IMesh {
	public:
		MeshD11(aiMesh* mesh, const aiScene* world);
		MeshD11(void* data, int count, VertexFormat vf);
		virtual ~MeshD11();

		virtual void Initialize(aiMesh* mesh) noexcept;
		virtual void Initialize(void* data, int count, VertexFormat vf) noexcept;
		virtual void Render(World* world, const Matrix4f& worldMatrix) noexcept;

	protected:
		unsigned int*		stride;
		unsigned int*		offset;
		ID3D11Buffer **		vbuffers;
		unsigned int		vbcount;
	};

}
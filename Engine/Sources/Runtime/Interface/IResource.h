#pragma once
#include "Runtime/Interface/IModule.h"

namespace scarlett {
	class GraphicsManager;
	class World;

	class IRenderResource{
	};

	enum VertexFormat
	{
		VF_None = 0,
		VF_P3F,
		VF_T2F,
		VF_N3F,
		VF_C4B,
	};

	class IVertexBuffer : public IRenderResource {
		virtual void Initialize(GraphicsManager* mgr, void* data, unsigned int count, VertexFormat vf) noexcept = 0;
		virtual unsigned int GetVertexSize(VertexFormat vf) noexcept = 0;
	};

	enum IndexFormat {
		IF_None = 0,
		IF_UINT16,
		IF_UINT32,
	};

	class IIndexBuffer : public IRenderResource {
		virtual void Initialize(GraphicsManager* mgr, void* data, unsigned int count, IndexFormat iformat) noexcept = 0;
	};


	enum PrimitiveType {
		PT_POINT = 1,
		PT_LINE,
		PT_TRIANGLE,
	};

	class IMesh : public IRenderResource {
	public:
		virtual void Render(GraphicsManager* mgr, World* world) noexcept = 0;

	};

}
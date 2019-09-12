#pragma once
#include <memory>
#include "Runtime/Interface/IModule.h"
#include "Runtime/Core/Math/ScltMath.h"
#include "assimp/scene.h"


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
		virtual void Initialize(void* data, unsigned int count, VertexFormat vf) noexcept = 0;
		virtual unsigned int GetVertexSize(VertexFormat vf) noexcept = 0;
		virtual void Finialize() noexcept = 0;
	};

	enum IndexFormat {
		IF_None = 0,
		IF_UINT16,
		IF_UINT32,
	};

	class IIndexBuffer : public IRenderResource {
		virtual void Initialize(void* data, unsigned int count, IndexFormat iformat) noexcept = 0;
		virtual void Finialize() noexcept = 0;
	};


	enum PrimitiveType {
		PT_POINT = 1,
		PT_LINE,
		PT_TRIANGLE,
	};

	class IMesh : public IRenderResource {
	public:
		virtual void		Initialize(aiMesh* mesh) noexcept = 0;
		virtual int			GetVaildVertexBufferCount() noexcept = 0;
		virtual void		Render(World* world, const Matrix4f& worldMatrix) noexcept = 0;
		virtual void		Finialize() noexcept = 0;

	};

	class Shader;
	class IMaterial : public IRenderResource {
	public:
		virtual void		Initialize() noexcept = 0;
		virtual void		Finialize() noexcept = 0;
		virtual	void		SetShader(std::shared_ptr<Shader>) = 0;
		virtual std::shared_ptr<Shader>		GetShader() = 0;
	};

}
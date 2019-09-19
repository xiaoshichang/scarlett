#pragma once
#include <memory>
#include <string>
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

	struct ConstantBuffer
	{
		Matrix4f world;
		Matrix4f view;
		Matrix4f projection;
		Vector4f debugColor;
	};

	enum DepthStencilStateWriteMask {
		ALL = 1,
		ZORE = 2,
	};

	enum DepthStencilStateFunc {
		NEVER = 1,
		ALWAYS = 2,
		LESS_EQUAL = 3,
		LESS = 4,
		EQUAL = 5,
		GREATER_EQUAL = 6,
		GREATER = 7,
		NOT_EQUAL = 8,
	};

	
	class IDepthStencilState : public IRenderResource {
	public:
		virtual void	SetWriteMask(DepthStencilStateWriteMask mask) = 0;
		virtual void	SetFunc(DepthStencilStateFunc func) = 0;

	};

	class Shader;
	class Texture;
	class SamplerState;
	class IMaterial : public IRenderResource {
	public:
		virtual void		Initialize() noexcept = 0;
		virtual void		Finialize() noexcept = 0;
		virtual	void		SetShader(std::shared_ptr<Shader>) = 0;
		virtual std::shared_ptr<Shader>		GetShader() = 0;
		virtual void		Apply(ConstantBuffer cb) noexcept = 0;
		virtual void		SetShaderParamter(std::string name, Vector4f value) noexcept = 0;
		virtual void		SetTexture(std::string name, std::shared_ptr<Texture> tex) noexcept = 0;
		virtual void		SerSamplerState(std::string name, std::shared_ptr<SamplerState>) noexcept = 0;
		virtual void		SetDepthStencilState(std::shared_ptr<IDepthStencilState>) = 0;
		virtual std::shared_ptr<IDepthStencilState>		GetDepthStencilState() = 0;
	};

	class ITexture : public IRenderResource {
	public:
		virtual void		Initialize(const std::string&) noexcept = 0;
		virtual void		Finialize() noexcept = 0;

	};

	class ISamplerState : public IRenderResource {
	public:
		virtual void		Initialize() noexcept = 0;
		virtual void		Finialize() noexcept = 0;
	};
}
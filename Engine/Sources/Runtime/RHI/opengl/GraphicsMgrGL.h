#pragma once
#include "Runtime/RHI/GraphicsMgr.h"
#include "Runtime/Core/Math/ScltMath.h"


namespace scarlett{
	
	class GraphicsMgrGL : public GraphicsManager {

	public:
		virtual		int		Initialize() noexcept;
		virtual		void	Finalize() noexcept;
		virtual		void	Tick() noexcept;

		virtual		void	Present() noexcept;
		virtual		void	ClearRenderTarget(float r, float g, float b, float a) noexcept;

		virtual		std::shared_ptr<VertexBuffer>	CreateVertexBuffer(void* data, int count, VertexFormat vf) noexcept;

		virtual		std::shared_ptr<IndexBuffer>	CreateIndexBuffer(void* data, int count, IndexFormat iformat) noexcept;

		virtual		std::shared_ptr<RenderMesh>		CreateRenderMesh(aiMesh* mesh, const aiScene* world) noexcept;
		virtual		std::shared_ptr<RenderMesh>		CreateRenderMeshDebug(void* data, int count, VertexFormat vf) noexcept;

		virtual		std::shared_ptr<Texture>		CreateTexture2D(const std::string& path) noexcept;
		virtual		std::shared_ptr<Texture>		CreateTextureCubemap(const std::string& path) noexcept;
		virtual		std::shared_ptr<SamplerState>	CreateSamplerState() noexcept;

		virtual		void	LoadShaders() noexcept;
		virtual		void	UseShader(std::shared_ptr<Shader>) noexcept;
		virtual		std::shared_ptr<Shader> GetShader(const std::string& shaderName) noexcept;

		virtual		void	Draw(unsigned int vcount, unsigned int start) noexcept;
		virtual		void	DrawIndexed(unsigned int icount, unsigned int start, int baseLoc) noexcept;
	};
}
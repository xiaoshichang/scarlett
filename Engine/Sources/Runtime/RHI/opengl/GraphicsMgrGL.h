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

		virtual		std::shared_ptr<IVertexBuffer>	CreateVertexBuffer(void* data, int count, VertexFormat vf) noexcept;

		virtual		std::shared_ptr<IIndexBuffer>	CreateIndexBuffer(void* data, int count, IndexFormat iformat) noexcept;

		virtual		std::shared_ptr<IMesh>		CreateRenderMesh() noexcept;
		virtual		std::shared_ptr<IMesh>		CreateRenderMesh(aiMesh* mesh, const aiScene* world) noexcept;
		virtual		std::shared_ptr<IMesh>		CreateRenderMeshDebug(void* data, int count, VertexFormat vf) noexcept;
		virtual		std::shared_ptr<IMesh>		CreateRenderMeshUI() noexcept;

		virtual		std::shared_ptr<ITexture>		CreateTexture2D(const std::string& path) noexcept;
		virtual		std::shared_ptr<ITexture>		CreateTexture2D(int width, int height, unsigned char* data) noexcept;
		virtual		std::shared_ptr<ITexture>		CreateTextureCubemap(const std::string& path) noexcept;
		virtual		std::shared_ptr<ISamplerState>	CreateSamplerState() noexcept;

		virtual		void	LoadShaders() noexcept;
		virtual		void	UseShader(std::shared_ptr<IShader>) noexcept;
		virtual		std::shared_ptr<IShader> GetShader(const std::string& shaderName) noexcept;

		virtual		void	Draw(unsigned int vcount, unsigned int start) noexcept;
		virtual		void	DrawIndexed(unsigned int icount, unsigned int start, int baseLoc) noexcept;
	};
}
#pragma once

#include <memory>
#include <unordered_map>

#include "Runtime/Interface/IModule.h"
#include "Runtime/Interface/IResource.h"
#include "Runtime/RHI/VertexBuffer.h"
#include "Runtime/RHI/IndexBuffer.h"
#include "Runtime/RHI/Shader.h"
#include "Runtime/RHI/RenderMesh.h"

#include "assimp/Importer.hpp"      // C++ importer interface
#include "assimp/scene.h"           // Output data structure

namespace scarlett{

	class GraphicsManager : public ITickableModule{
	public:
		virtual		int		Initialize() noexcept = 0;
		virtual		void	Finalize() noexcept = 0;

		virtual		void	Present() noexcept = 0;
		virtual		void	ClearRenderTarget(float r, float g, float b, float a) noexcept = 0;

		virtual		std::shared_ptr<VertexBuffer>	CreateVertexBuffer(void* data, int count, VertexFormat vf) noexcept = 0;

		virtual		std::shared_ptr<IndexBuffer>	CreateIndexBuffer(void* data, int count, IndexFormat iformat) noexcept = 0;

		virtual		std::shared_ptr<RenderMesh>		CreateRenderMesh(aiMesh* mesh, const aiScene* world) noexcept = 0;
		virtual		std::shared_ptr<RenderMesh>		CreateRenderMeshDebug(void* data, int count, VertexFormat vf) noexcept = 0;

		virtual		std::shared_ptr<Texture>		CreateTexture2D(const std::string& path) noexcept = 0;
		virtual		std::shared_ptr<SamplerState>	CreateSamplerState() noexcept = 0;

		virtual		void	LoadShaders() noexcept = 0;
		virtual		void	UseShader(std::shared_ptr<Shader>) noexcept = 0;
		virtual		std::shared_ptr<Shader> GetShader(const std::string& shaderName) noexcept = 0;

		virtual		void	Draw(unsigned int vcount, unsigned int start) noexcept = 0;
		virtual		void	DrawIndexed(unsigned int icount, unsigned int start, int baseLoc) noexcept = 0;

	protected:
		std::unordered_map<std::string, std::shared_ptr<Shader>>	mShaders;
		std::unordered_map<std::string, std::shared_ptr<Texture>>	mTextures;

	};

}
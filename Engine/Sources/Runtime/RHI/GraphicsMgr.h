#pragma once

#include <memory>

#include "Runtime/Interface/IModule.h"
#include "Runtime/Interface/IResource.h"
#include "Runtime/RHI/VertexBuffer.h"
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
		virtual		void	DeleteVertexBuffer(std::shared_ptr<VertexBuffer> vb) noexcept = 0;

		virtual		std::shared_ptr<RenderMesh>		CreateRenderMesh(aiMesh* mesh) noexcept = 0;
		virtual		void	DeleteRenderMesh(std::shared_ptr<RenderMesh> mesh) noexcept = 0;


	};

}
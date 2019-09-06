#pragma once
#include "Runtime/RHI/GraphicsMgr.h"
#include "Runtime/Core/Math/ScltMath.h"
#include "Runtime/Utils/Singleton.h"
#include <d3d11.h>
#include <Windows.h>

#define SAVE_RELEASE_DXOBJ(OBJ) {OBJ->Release();OBJ=0;}


namespace scarlett {

	class GraphicsMgrD11 : public GraphicsManager {

	public:
		virtual		int		Initialize() noexcept;
		virtual		int		InitializeWithWindow(HWND handler) noexcept;
		virtual		void	Finalize() noexcept;
		virtual		void	Tick() noexcept;

		virtual		void	Present() noexcept;
		virtual		void	ClearRenderTarget(float r, float g, float b, float a) noexcept;

		virtual		std::shared_ptr<VertexBuffer>	CreateVertexBuffer(void* data, int count, VertexFormat vf) noexcept;
		virtual		void	DeleteVertexBuffer(std::shared_ptr<VertexBuffer> vb) noexcept;

		virtual		std::shared_ptr<RenderMesh>		CreateRenderMesh(aiMesh* mesh) noexcept;
		virtual		void	DeleteRenderMesh(std::shared_ptr<RenderMesh> mesh) noexcept;

		virtual		void	LoadShaders() noexcept;
		virtual		void	UseShader(const std::string& shaderName) noexcept;

	protected:
		HWND m_hwnd;

		char m_videoCardDescription[128];
		int m_videoCardMemory;

		IDXGISwapChain*				m_swapChain;
		ID3D11Device*				m_device;
		ID3D11DeviceContext*		m_deviceContext;
		ID3D11RenderTargetView*		m_renderTargetView;
		ID3D11Texture2D*			m_depthStencilBuffer;
		ID3D11DepthStencilState*	m_depthStencilState;
		ID3D11DepthStencilView*		m_depthStencilView;
		ID3D11RasterizerState*		m_rasterState;

	};

}
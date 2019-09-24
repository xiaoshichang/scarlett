#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Runtime/RHI/D11/VertexBufferD11.h"
#include "Runtime/RHI/D11/IndexBufferD11.h"
#include "Runtime/RHI/D11/ShaderD11.h"
#include "Runtime/RHI/D11/MeshD11.h"
#include "Runtime/RHI/D11/TextureD11.h"
#include "Runtime/RHI/D11/SamplerStateD11.h"
#include "Foundation/Assert.h"
#include "Runtime/Utils/Logging.h"
#include <iostream>

using namespace scarlett;


int GraphicsMgrD11::Initialize() noexcept {
	SCARLETT_ASSERT(false);
	return 0;
}

int GraphicsMgrD11::InitializeWithWindow(HWND handler) noexcept
{
	m_hwnd = handler;

	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	size_t stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	SCARLETT_ASSERT(result >= 0);

	// Enumerate the primary adapter output (monitor).
	result = factory->EnumAdapters(0, &adapter);
	SCARLETT_ASSERT(result >= 0);


	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	SCARLETT_ASSERT(result >= 0);

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	SCARLETT_ASSERT(result >= 0);

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	SCARLETT_ASSERT(displayModeList);

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	SCARLETT_ASSERT(result >= 0);

	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)1024)
		{
			if (displayModeList[i].Height == (unsigned int)768)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	result = adapter->GetDesc(&adapterDesc);
	SCARLETT_ASSERT(result >= 0);

	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	SCARLETT_ASSERT(error == 0);
	SCARLETT_LOG(info) << "video card description: " << m_videoCardDescription;
	SCARLETT_LOG(info) << "video card memory: " << m_videoCardMemory;

	delete[] displayModeList;
	displayModeList = 0;

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = 1024;
	swapChainDesc.BufferDesc.Height = 748;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if (false)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = m_hwnd;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (false)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &m_device, NULL, &m_deviceContext);
	SCARLETT_ASSERT(result >= 0);

	UINT x4MsaaQuality;
	result = m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 8, &x4MsaaQuality);
	SCARLETT_ASSERT(result >= 0);

	result = m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, 8, &x4MsaaQuality);
	SCARLETT_ASSERT(result >= 0);

	result = factory->CreateSwapChain(m_device, &swapChainDesc, &m_swapChain);
	SCARLETT_ASSERT(result >= 0);

	// Get the pointer to the back buffer.
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	SCARLETT_ASSERT(result >= 0);
	// Create the render target view with the back buffer pointer.
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	SCARLETT_ASSERT(result >= 0);

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = 1024;
	depthBufferDesc.Height = 768;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 4;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	SCARLETT_ASSERT(result >= 0);

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	SCARLETT_ASSERT(result >= 0);
	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;	//Your dimension for the DSV needs to be D3D11_DSV_DIMENSION_TEXTURE2DMS if the Texture2D was created with multisampling.
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	SCARLETT_ASSERT(result >= 0);

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = true;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	SCARLETT_ASSERT(result >= 0);

	// Now set the rasterizer state.
	m_deviceContext->RSSetState(m_rasterState);

	// Setup the viewport for rendering.
	viewport.Width = (float)1024;
	viewport.Height = (float)768;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	// Create the viewport.
	m_deviceContext->RSSetViewports(1, &viewport);

	LoadShaders();

	SAVE_RELEASE_DXOBJ(adapterOutput);
	SAVE_RELEASE_DXOBJ(adapter);
	SAVE_RELEASE_DXOBJ(factory);
	return 0;
}


void GraphicsMgrD11::Finalize() noexcept {
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}
	SAVE_RELEASE_DXOBJ(m_rasterState);
	SAVE_RELEASE_DXOBJ(m_depthStencilView);
	SAVE_RELEASE_DXOBJ(m_depthStencilState);
	SAVE_RELEASE_DXOBJ(m_depthStencilBuffer);
	SAVE_RELEASE_DXOBJ(m_renderTargetView);
	SAVE_RELEASE_DXOBJ(m_deviceContext);
	SAVE_RELEASE_DXOBJ(m_device);
	SAVE_RELEASE_DXOBJ(m_swapChain);
}

void GraphicsMgrD11::Tick() noexcept {

}

void GraphicsMgrD11::ClearRenderTarget(float r, float g, float b, float a) noexcept {
	float color[4] = { r, g, b, a };
	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// Clear the depth buffer.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

std::shared_ptr<IVertexBuffer> scarlett::GraphicsMgrD11::CreateVertexBuffer(void * data, int count, VertexFormat vf) noexcept
{
	auto ptr = std::make_shared<VertexBufferD11>(data, count, vf);
	return ptr;
}


std::shared_ptr<IIndexBuffer> scarlett::GraphicsMgrD11::CreateIndexBuffer(void * data, int count, IndexFormat iformat) noexcept
{
	auto ptr = std::make_shared<IndexBufferD11>(data, count, iformat);
	return ptr;
}


std::shared_ptr<IMesh> scarlett::GraphicsMgrD11::CreateRenderMesh(aiMesh * mesh, const aiScene* world) noexcept
{
	auto ptr = std::make_shared<MeshD11>(mesh, world);
	return ptr;
}

std::shared_ptr<IMesh> scarlett::GraphicsMgrD11::CreateRenderMeshDebug(void* data, int count, VertexFormat vf) noexcept
{
	auto ptr = std::make_shared<MeshD11>(data, count, vf);
	return ptr;
}

std::shared_ptr<ITexture> scarlett::GraphicsMgrD11::CreateTexture2D(const std::string& path) noexcept
{
	if (mTextures[path]) {
		return mTextures[path];
	}

	auto ptr = std::make_shared<TextureD11>(path, TextureType::Default);
	mTextures[path] = ptr;
	return ptr;
}

std::shared_ptr<ITexture> scarlett::GraphicsMgrD11::CreateTextureCubemap(const std::string & path) noexcept
{
	if (mTextures[path]) {
		return mTextures[path];
	}

	auto ptr = std::make_shared<TextureD11>(path, TextureType::Cubemap);
	mTextures[path] = ptr;
	return ptr;
}

std::shared_ptr<ISamplerState> scarlett::GraphicsMgrD11::CreateSamplerState() noexcept
{
	auto ptr = std::make_shared<SamplerStateD11>();
	return ptr;
}


void scarlett::GraphicsMgrD11::LoadShaders() noexcept
{
	std::string pbrShaderVS = "Asset/Shaders/pbr.vs";
	std::string pbrShaderPS = "Asset/Shaders/pbr.ps";
	auto pbrShader = std::make_shared<ShaderD11>(pbrShaderVS, pbrShaderPS);
	mShaders["pbr"] = pbrShader;

	std::string debugShaderVS = "Asset/Shaders/debug.vs";
	std::string debugShaderPS = "Asset/Shaders/debug.ps";
	auto debugShader = std::make_shared<ShaderD11>(debugShaderVS, debugShaderPS);
	mShaders["debug"] = debugShader;

	std::string pbr_skinVS = "Asset/Shaders/pbr_skin.vs";
	std::string pbr_skinPS = "Asset/Shaders/pbr_skin.ps";
	auto pbr_skin = std::make_shared<ShaderD11>(pbr_skinVS, pbr_skinPS);
	mShaders["pbr_skin"] = pbr_skin;

	std::string skyboxvs = "Asset/Shaders/skybox.vs";
	std::string skyboxps = "Asset/Shaders/skybox.ps";
	auto skybox = std::make_shared<ShaderD11>(skyboxvs, skyboxps);
	mShaders["skybox"] = skybox;
}

void scarlett::GraphicsMgrD11::UseShader(std::shared_ptr<IShader> shader) noexcept
{
	if (!shader) {
		SCARLETT_ASSERT(false);
	}
	shader->Use();
}

std::shared_ptr<IShader> scarlett::GraphicsMgrD11::GetShader(const std::string & shaderName) noexcept
{
	return mShaders[shaderName];
}

void scarlett::GraphicsMgrD11::Draw(unsigned int vcount, unsigned int start) noexcept{
	m_deviceContext->Draw(vcount, start);
}

void scarlett::GraphicsMgrD11::DrawIndexed(unsigned int icount, unsigned int start, int baseLoc) noexcept{
	m_deviceContext->DrawIndexed(icount, start, baseLoc);
}

void GraphicsMgrD11::Present() noexcept {
	if (false){
		// Lock to screen refresh rate.
		m_swapChain->Present(1, 0);
	}
	else{
		// Present as fast as possible.
		m_swapChain->Present(0, 0);
	}
	return;
}
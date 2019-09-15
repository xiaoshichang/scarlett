#include "TextureD11.h"
#include "Foundation/Assert.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/Utils/ImageParser.h"

scarlett::TextureD11::TextureD11(const std::string& filepath)
{
	Initialize(filepath);
}

scarlett::TextureD11::~TextureD11()
{
	Finialize();
}

void scarlett::TextureD11::Initialize(const std::string& filepath) noexcept
{
	HRESULT result;
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	int width, height, channels;
	auto data = scarlett::ImageParser::Parse(filepath, &width, &height, &channels, 4);

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = data;
	sd.SysMemPitch = width * sizeof(uint32_t);
	sd.SysMemSlicePitch = width * height * sizeof(uint32_t);

	result = mgrd11->m_device->CreateTexture2D(&texDesc, &sd, &mTexture);
	if (FAILED(result)) {
		SCARLETT_ASSERT(false);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	result = mgrd11->m_device->CreateShaderResourceView(mTexture, &srvDesc, &mView);
	if (FAILED(result)) {
		SCARLETT_ASSERT(false);
	}
}

void scarlett::TextureD11::Finialize() noexcept {
	if (mView) {
		mView->Release();
	}
	if (mTexture) {
		mTexture->Release();
	}
	mView = nullptr;
	mTexture = nullptr;
	
}
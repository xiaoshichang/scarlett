#include "TextureD11.h"
#include "Foundation/Assert.h"
#include "Runtime/RHI/D11/GraphicsMgrD11.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/RHI/D11/DDSTextureLoader.h"
#include <codecvt>

std::wstring stringToWstring(const std::string& t_str)
{
	//setup converter
	typedef std::codecvt_utf8<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;
	return converter.from_bytes(t_str);
}


scarlett::TextureD11::TextureD11(const std::string& filepath, TextureType type):
	ITexture(type)
{
	if (mType == TextureType::Default) {
		InitializeDefault(filepath);
	}
	else {
		InitializeCubemap(filepath);
	}
}

scarlett::TextureD11::TextureD11(int width, int height, unsigned char * data):
	ITexture(TextureType::Default)
{
	HRESULT result;
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
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

scarlett::TextureD11::~TextureD11()
{
	if (mView) {
		mView->Release();
	}
	if (mTexture) {
		mTexture->Release();
	}
	mView = nullptr;
	mTexture = nullptr;
}


void scarlett::TextureD11::InitializeDefault(const std::string & filepath) noexcept
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

void scarlett::TextureD11::InitializeCubemap(const std::string & filepath) noexcept
{
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	auto wpath = stringToWstring(filepath);
	auto hr = DirectX::CreateDDSTextureFromFile(mgrd11->m_device, mgrd11->m_deviceContext, wpath.c_str(), nullptr, &mView);
}
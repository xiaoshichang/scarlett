#pragma once

#include "Runtime/RHI/Texture.h"
#include "Runtime/Utils/ImageParser.h"
#include <d3d11.h>

namespace scarlett {

	class TextureD11 : public ITexture {
	public:
		TextureD11(const std::string& filepath, TextureType type);
		virtual ~TextureD11();

		virtual void	InitializeDefault(const std::string& filepath) noexcept;
		virtual void	InitializeCubemap(const std::string& filepath) noexcept;

	public:
		ID3D11Texture2D*			mTexture;
		ID3D11ShaderResourceView*	mView;
	};
}
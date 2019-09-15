#pragma once

#include "Runtime/RHI/Texture.h"
#include <d3d11.h>

namespace scarlett {

	class TextureD11 : public Texture {
	public:
		TextureD11(const std::string& filepath);
		virtual ~TextureD11();
		virtual void	Initialize(const std::string& filepath) noexcept;
		virtual void	Finialize() noexcept;

	public:
		ID3D11Texture2D*			mTexture;
		ID3D11ShaderResourceView*	mView;
	};
}
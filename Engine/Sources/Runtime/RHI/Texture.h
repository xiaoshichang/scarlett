#pragma once
#include <string>
#include "Runtime/Interface/IResource.h"

namespace scarlett {

	enum TextureType {
		Default = 1,
		Cubemap = 2,
	};

	class Texture : public ITexture {

	public:

		virtual void	Initialize(const std::string& filepath) noexcept;

		virtual void	InitializeDefault(const std::string& filepath) noexcept;
		virtual void	InitializeCubemap(const std::string& filepath) noexcept;

		virtual void	Finialize() noexcept;

	public:
		TextureType mType;
	};

}
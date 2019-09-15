#pragma once
#include <string>
#include "Runtime/Interface/IResource.h"

namespace scarlett {

	class Texture : public ITexture {

	public:

		virtual void	Initialize(const std::string& filepath) noexcept;
		virtual void	Finialize() noexcept;

	public:

	};

}
#pragma once

#include "Runtime/Interface/IResource.h"
#include "Runtime/RHI/Shader.h"

namespace scarlett {
	
	class Material : IMaterial {
	public:
		virtual void		Initialize() noexcept;
		virtual void		Finialize() noexcept;
		virtual	void		SetShader(std::shared_ptr<Shader>);
		virtual std::shared_ptr<Shader>		GetShader();

	private:
		std::shared_ptr<Shader>		mShader;


	};

}
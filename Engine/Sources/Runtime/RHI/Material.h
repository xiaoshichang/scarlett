#pragma once
#include <unordered_map>
#include "Runtime/Interface/IResource.h"
#include "Runtime/RHI/Shader.h"

namespace scarlett {
	
	class Material : IMaterial {
	public:
		virtual void		Initialize() noexcept;
		virtual void		Finialize() noexcept;
		virtual	void		SetShader(std::shared_ptr<Shader>);
		virtual std::shared_ptr<Shader>		GetShader();
		virtual void		Apply(ConstantBuffer cb) noexcept;
		virtual void		SetShaderParamter(std::string name, Vector4f value) noexcept;

	private:
		std::shared_ptr<Shader>						mShader;
		std::unordered_map<std::string, Vector4f>	mParameters;
	};

}
#pragma once
#include <unordered_map>
#include "Runtime/Interface/IResource.h"
#include "Runtime/RHI/Shader.h"
#include "Runtime/RHI/Texture.h"
#include "Runtime/RHI/SamplerState.h"

namespace scarlett {
	
	class Material : IMaterial {
	public:
		virtual void		Initialize() noexcept;
		virtual void		Finialize() noexcept;
		virtual	void		SetShader(std::shared_ptr<Shader>);
		virtual std::shared_ptr<Shader>		GetShader();
		virtual void		Apply(ConstantBuffer cb) noexcept;
		virtual void		SetShaderParamter(std::string name, Vector4f value) noexcept;
		virtual void		SetTexture(std::string name, std::shared_ptr<Texture> tex) noexcept;
		virtual void		SerSamplerState(std::string name, std::shared_ptr<SamplerState>) noexcept;

	protected:
		std::shared_ptr<Shader>						mShader;
		std::unordered_map<std::string, Vector4f>	mParameters;
		std::unordered_map<std::string, std::shared_ptr<Texture>>	mTextures;
		std::unordered_map<std::string, std::shared_ptr<SamplerState>>	mSamplerState;
	};

}
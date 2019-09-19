#pragma once
#include <unordered_map>
#include "Runtime/Interface/IResource.h"
#include "Runtime/RHI/Shader.h"
#include "Runtime/RHI/Texture.h"
#include "Runtime/RHI/SamplerState.h"

namespace scarlett {
	
	enum MaterialType {
		Surface = 1,
		Skybox = 2,
	};

	class Material : IMaterial {
	public:
		Material(): mType(MaterialType::Surface), mShader(nullptr){}
		virtual void			Initialize() noexcept;
		virtual void			Finialize() noexcept;
		virtual	void			SetShader(std::shared_ptr<Shader>);
		virtual std::shared_ptr<Shader>		GetShader();
		virtual void			Apply(ConstantBuffer cb) noexcept;
		virtual void			SetShaderParamter(std::string name, Vector4f value) noexcept;
		virtual void			SetTexture(std::string name, std::shared_ptr<Texture> tex) noexcept;
		virtual void			SerSamplerState(std::string name, std::shared_ptr<SamplerState>) noexcept;
		virtual void			SetType(MaterialType type) { mType = type; }
		virtual MaterialType	GetType() { return mType; }
		virtual void			ApplySurface(ConstantBuffer cb) = 0;
		virtual void			ApplySkybox(ConstantBuffer cb) = 0;
		virtual void			SetDepthStencilState(std::shared_ptr<IDepthStencilState> state) { mDepthStencilState = state; }
		virtual std::shared_ptr<IDepthStencilState>		GetDepthStencilState() { return mDepthStencilState; }


	protected:
		MaterialType		mType;
		std::shared_ptr<Shader>						mShader;
		std::shared_ptr<IDepthStencilState>			mDepthStencilState;
		std::unordered_map<std::string, Vector4f>	mParameters;
		std::unordered_map<std::string, std::shared_ptr<Texture>>	mTextures;
		std::unordered_map<std::string, std::shared_ptr<SamplerState>>	mSamplerState;
	};

}
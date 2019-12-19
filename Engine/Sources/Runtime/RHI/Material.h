#pragma once
#include <Boost/unordered_map.hpp>
#include "Runtime/Interface/IResource.h"
#include "Runtime/RHI/Shader.h"
#include "Runtime/RHI/SamplerState.h"
#include "Runtime/RHI/DepthStencilState.h"
#include "Runtime/RHI/Texture.h"
#include "Runtime/RHI/BlendState.h"


namespace scarlett {
	
	class IMaterial : public IRenderResource {
	public:
		virtual void		Apply(ConstantBuffer cb) noexcept = 0;
		virtual void		ApplyLight(ConstantBufferLighting cb) noexcept = 0;
		virtual void		ApplyAnimation(ConstantBufferAnimation cb) noexcept = 0;

		void SetName(std::string name) { mName = name; }
		std::string GetName() { return mName; }

		void SetShaderParamter(std::string name, Vector4f value) { mParameters[name] = value; }
		Vector4f getShaderParamter(std::string name) { return mParameters[name]; }

		void SetTexture(std::string name, std::shared_ptr<ITexture> tex) { mTextures[name] = tex; }
		std::shared_ptr<ITexture> getTexture(std::string name) { return mTextures[name]; }

		void SetSamplerState(std::string name, std::shared_ptr<ISamplerState> state) { mSamplerState[name] = state; }
		std::shared_ptr<ISamplerState> getSamplerState(std::string name) { return mSamplerState[name]; }

		void SetShader(std::shared_ptr<IShader> shader) { mShader = shader; }
		std::shared_ptr<IShader>		GetShader() { return mShader; }

		void SetDepthStencilState(std::shared_ptr<IDepthStencilState> state) { mDepthStencilState = state; }
		std::shared_ptr<IDepthStencilState>	GetDepthStencilState() { return mDepthStencilState; }

		void SetBlendState(std::shared_ptr<IBlendState> state) { mBlendState = state; }
		std::shared_ptr<IBlendState> GetBlendState() { return mBlendState; }

	protected:
		std::string															mName;
		std::shared_ptr<IShader>											mShader;
		std::shared_ptr<IDepthStencilState>									mDepthStencilState;
		std::shared_ptr<IBlendState>										mBlendState;
		boost::unordered_map<std::string, Vector4f>							mParameters;

		boost::unordered_map<std::string, std::shared_ptr<ITexture>>		mTextures;
		boost::unordered_map<std::string, std::shared_ptr<ISamplerState>>	mSamplerState;
	};

}
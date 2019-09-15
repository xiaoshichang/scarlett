#include "Material.h"

void scarlett::Material::Initialize() noexcept
{
}

void scarlett::Material::Finialize() noexcept
{
}

void scarlett::Material::SetShader(std::shared_ptr<Shader> shader)
{
	mShader = shader;
}

std::shared_ptr<scarlett::Shader> scarlett::Material::GetShader()
{
	return mShader;
}

void scarlett::Material::Apply(ConstantBuffer cb) noexcept
{
}

void scarlett::Material::SetShaderParamter(std::string name, Vector4f value) noexcept
{
	mParameters[name] = value;
}

void scarlett::Material::SetTexture(std::string name, std::shared_ptr<Texture> tex) noexcept
{
	mTextures[name] = tex;
}

void scarlett::Material::SerSamplerState(std::string name, std::shared_ptr<SamplerState> sampler) noexcept
{
	mSamplerState[name] = sampler;
}

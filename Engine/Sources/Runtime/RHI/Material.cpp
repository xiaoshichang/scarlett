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
	mShader->Use();
	for (auto pair : mParameters) {
		if (pair.first == "color") {
			cb.debugColor = pair.second;
		}
	}
	mShader->SetConstantBuffer(cb);
}

void scarlett::Material::SetShaderParamter(std::string name, Vector4f value) noexcept
{
	mParameters[name] = value;
}
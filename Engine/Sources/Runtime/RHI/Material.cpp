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

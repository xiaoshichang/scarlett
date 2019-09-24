#pragma once

#include "Runtime/RHI/Shader.h"
#include "glad/glad_wgl.h"

namespace scarlett {

	class ShaderGL : public IShader {
	public:
		ShaderGL(const std::string & vsPath, const std::string & psPath);
		virtual ~ShaderGL();
	public:
		virtual	bool	InitializeFromFile(const std::string& vsPath, const std::string& psPath) noexcept;
		virtual	void	Use() noexcept;
		virtual void	SetConstantBuffer(const ConstantBuffer& cbuffer) noexcept;

	public:

		GLuint mProgram;

	};

}
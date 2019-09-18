#pragma once

#include "Runtime/RHI/Shader.h"
#include "glad/glad_wgl.h"

namespace scarlett {

	class ShaderGL : public Shader {
	public:
		ShaderGL(const string & vsPath, const string & psPath);
		virtual ~ShaderGL();
	public:
		virtual	bool	InitializeFromFile(const string& vsPath, const string& psPath) noexcept;
		virtual	void	Use() noexcept;
		virtual void	Finialize() noexcept;
		virtual void	SetConstantBuffer(const ConstantBuffer& cbuffer) noexcept;

	public:

		GLuint mProgram;

	};

}
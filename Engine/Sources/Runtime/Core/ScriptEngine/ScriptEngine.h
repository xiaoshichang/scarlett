#pragma once

#include <string>
#include "Runtime/Interface/IModule.h"

extern "C"
{
#include "Runtime/Core//ScriptEngine/lua-5.4.2/src/lualib.h"
#include "Runtime/Core//ScriptEngine/lua-5.4.2/src/lauxlib.h"
}


namespace scarlett
{
	class IScriptEngine : public IModule
	{
	public:
		virtual int Eval(std::string cmd) = 0;

	};

	class ScriptEngineLua : public IScriptEngine
	{
	public:
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;
		virtual int Eval(std::string cmd);

	private:
		lua_State* L;


	};

}
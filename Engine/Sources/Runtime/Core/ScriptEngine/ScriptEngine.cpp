#include "ScriptEngine.h"


int scarlett::ScriptEngineLua::Initialize() noexcept
{
	L = luaL_newstate();
	luaL_openlibs(L);
	return 0;
}

void scarlett::ScriptEngineLua::Finalize() noexcept
{

}

int scarlett::ScriptEngineLua::Eval(std::string cmd)
{
	int ret = luaL_dostring(L, cmd.c_str());

	if (ret != LUA_OK)
	{
		// https://stackoverflow.com/questions/56230859/how-to-properly-print-error-messages-from-lual-dostring/56235607
		printf("[ScriptEngineLua]: %s\n", lua_tostring(L, -1));
		lua_pop(L, 1); // pop error message
		return false;
	}

	return 0;
}

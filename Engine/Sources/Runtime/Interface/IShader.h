#pragma once
#include <string>
using namespace std;

namespace scarlett {
	class GraphicsMgr;
	class IShader {
	public:
		virtual	bool	InitializeFromFile(GraphicsMgr* mgr, const string& vsPath, const string& psPath) noexcept = 0;
		virtual	void	Use(GraphicsMgr* mgr) noexcept = 0;

	};

}
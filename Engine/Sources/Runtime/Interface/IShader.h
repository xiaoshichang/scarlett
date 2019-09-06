#pragma once
#include <string>
using namespace std;

namespace scarlett {
	class GraphicsManager;
	class IShader {
	public:
		virtual	bool	InitializeFromFile(GraphicsManager* mgr, const string& vsPath, const string& psPath) noexcept = 0;
		virtual	void	Use(GraphicsManager* mgr) noexcept = 0;

	};

}
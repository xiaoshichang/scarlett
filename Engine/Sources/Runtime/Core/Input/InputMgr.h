#pragma once

#include "Runtime/Interface/IModule.h"

namespace scarlett {
#define KEY_CODE_ESC	0x1B
#define KEY_CODE_W		0x57
#define KEY_CODE_S		0x53
#define KEY_CODE_A		0x41
#define KEY_CODE_D		0x44

#define KEY_CODE_LBUTTON 0x01
#define KEY_CODE_RBUTTON 0x02

	class InputMgr : public IModule {
	public:
		virtual int		Initialize()	noexcept;
		virtual void	Finalize()		noexcept;

		void	OnKeyDown(unsigned char keyCode);
		void	OnKeyUp(unsigned char keyCode);
		void	OnMouseMove(int x, int y);

	};
}
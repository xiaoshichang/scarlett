#pragma once
#include <thread>
#include <chrono>
#include "Runtime/Interface/IModule.h"

namespace scarlett {

	class TimeMgr : ITickableModule {
	public:
		virtual int		Initialize() noexcept;
		virtual void	Finalize() noexcept;
		virtual void	Tick() noexcept;

		void	PreTick() noexcept;
		void	PostTick() noexcept;

		void	MicroSleep(unsigned long long ns) noexcept;
		float	GetTotalMsTime() noexcept;
		float	GetDeltaMsTime() noexcept;

	private:
		bool m_bFirstUpdate;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_lastUpdateTime;

		std::chrono::time_point<std::chrono::high_resolution_clock> m_frameStartTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_frameEndTime;

		std::chrono::time_point<std::chrono::high_resolution_clock> m_LastStatisticTime;
		int m_frameCount;
		float m_fFPS;
		std::chrono::nanoseconds m_deltaTime;
	};

}
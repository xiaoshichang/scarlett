#include "TimeMgr.h"
#include "Runtime/Utils/Logging.h"
#include <iostream>

int scarlett::TimeMgr::Initialize() noexcept
{
	SCARLETT_LOG(info) << "TimerMgr Initialize ok.";
	m_bFirstUpdate = true;
	m_startTime = m_LastStatisticTime = std::chrono::high_resolution_clock::now();
	m_frameCount = 0;
	return 0;
}

void scarlett::TimeMgr::Finalize() noexcept
{
}

void scarlett::TimeMgr::Tick() noexcept
{
	auto duration = m_frameStartTime - m_LastStatisticTime;
	if (duration > std::chrono::seconds(1)) {
		m_fFPS = m_frameCount;
		m_frameCount = 0;
		m_LastStatisticTime = m_frameStartTime;
	}
	else {
		m_frameCount++;
	}
}

void scarlett::TimeMgr::PreTick() noexcept
{
	if (m_bFirstUpdate) {
		m_lastUpdateTime = std::chrono::high_resolution_clock::now();
		m_bFirstUpdate = false;
	}

	m_frameStartTime = std::chrono::high_resolution_clock::now();
	m_deltaTime = m_frameStartTime - m_lastUpdateTime;
	m_lastUpdateTime = m_frameStartTime;
}

void scarlett::TimeMgr::PostTick() noexcept
{
	m_frameEndTime = std::chrono::high_resolution_clock::now();
	auto total = m_frameEndTime - m_frameStartTime;
	long long interval = 16666666;
	long long sleep = interval - total.count();
	if (sleep > 0) {
		MicroSleep(sleep);
	}
}

void scarlett::TimeMgr::MicroSleep(unsigned long long ns) noexcept
{
	std::this_thread::sleep_for(std::chrono::nanoseconds(ns));
}

float scarlett::TimeMgr::GetTotalMsTime() noexcept
{
	auto delta = m_frameStartTime - m_startTime;
	return delta.count() / 1000000;
}

#pragma once


namespace scarlett
{
	class IIntegratePhase
	{
	public:
		virtual void integrate() = 0;
	};

	class IntegratePhase : public IIntegratePhase
	{
	public:
		virtual void integrate();
	};
}
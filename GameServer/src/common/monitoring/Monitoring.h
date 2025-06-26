#pragma once

/*================================
		  Monitoring
================================*/

#include "../macro/Define.h"

class Monitoring
{
	SINGLETON_BASE(Monitoring);

public:
	void Init();
	double ProcessCPUUsage();       
	size_t ProcessMemUsage() const;
	size_t PhysicMemUsage() const;

private:
	ULARGE_INTEGER _lastCPU{};
	ULARGE_INTEGER _lastSysCPU{};
	ULARGE_INTEGER _lastUserCPU{};

	int _processors{};
	HANDLE _self{};
};

#define MonitorManager Monitoring::GetInstance()
